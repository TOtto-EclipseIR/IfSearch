#include "VigilBehavior.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>

#include <eirExe/Log.h>
#include <eirExe/ResultId.h>
#include <eirNetwork/HttpManager.h>
#include <eirFrameSource/FrameSourceState.h>
#include <eirFrameSource/FrameSourceResult.h>

#include "VigilPlugin.h"

VigilBehavior::VigilBehavior(void)
    : _manager(0)
    , _reply(0)
    , currentGet_mst(MillisecondTime::current())
{
}

FrameSourceBehavior * VigilBehavior::brother(void)
{
    return new VigilBehavior();
}

QString VigilBehavior::name(void) const
{
    return "vigil";
}

QStringList VigilBehavior::schemeList(void)
{
    FUNCTION();
    FNRETURN("vigilLive vigilPlay" );
    return QStringList() << "vigilLive" << "vigilPlay";
}

bool VigilBehavior::supports(const QUrl & url)
{
    FUNCTION();
    bool b = ("vigilLive" == url.scheme() || "vigilPlay" == url.scheme());
    FNRETURN(b);
    return b;
}

bool VigilBehavior::supportsScheme(const QString & scheme)
{
    FUNCTION();
    bool b = ("vigilLive" == scheme || "vigilPlay" == scheme);
    FNRETURN(b);
    return b;
}

/*--- common ---*/

void VigilBehavior::getResult(MillisecondTime mst,
                              bool isError,
                              ResultSet rset)
{
    FUNCTION(qint64(mst), isError, rset.size());
    RESULTS(rset);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    if (FrameSourceState::Grabbing == _plugin->state()
            && "vigilLive" == _plugin->url().scheme())
        handleLiveGrab(mst, isError, rset);
    else if (FrameSourceState::Grabbing == _plugin->state()
                 && "vigilPlay" == _plugin->url().scheme())
        handlePlayGrab(mst, isError, rset);
    else if (FrameSourceState::Starting == _plugin->state()
             && "vigilPlay" == _plugin->url().scheme())
        handlePlayStarting(mst, isError, rset);
    else
        WATCH("Unhandled getResult()");
}

/*===== Construct =====*/

void VigilBehavior::enterConstruct(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    _socket = new QTcpSocket(_plugin);
    _manager = new HttpManager(_plugin);
    OBJPOINTER(QTcpSocket, _socket);
    OBJPOINTER(HttpManager, _manager);
    CONNECT(_socket, SIGNAL(connected()),
            _plugin, SLOT(connectResult()));
    CONNECT(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            _plugin, SLOT(connectResult()));
    ResultSet rset;
    rset << Result(FrameSourceResult::Initialized, name());
    _plugin->emitInitialized(rset);
}

/*===== Connect =====*/

void VigilBehavior::enterConnecting(void)
{
    FUNCTION();
    OBJPOINTER(QTcpSocket, _socket);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    _socket->connectToHost(_plugin->url().host(),
                           _plugin->url().port(80));
    TRACE("connectToHost(%1, %2)", _plugin->url().host(),
          _plugin->url().port(80));
}

void VigilBehavior::connectResult(void)
{
    FUNCTION();
    if (EXPECTNE(0, _socket)) return;
    OBJPOINTER(QTcpSocket, _socket);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    ResultSet rset;

    if (EXPECTEQ(QAbstractSocket::ConnectedState, _socket->state()))
    {
        rset.add(Result(FrameSourceResult::HttpConnectSuccess,
                       _plugin->url().toString()));
        _plugin->emitConnected(rset);
        _socket->disconnectFromHost();
    }
    else
    {
        rset.add(Result(FrameSourceResult::HttpConnectFailure,
                       _plugin->url(),
                       _socket->state(),
                       _socket->error(),
                       _socket->errorString()));
        _plugin->emitConnectError(rset);
    }

    RESULTS(rset);
}

void VigilBehavior::exitConnecting(void)
{
    FUNCTION();
    currentGet_mst = MillisecondTime::null();

    if ( ! EXPECTNE(0, _socket)) return;
    OBJPOINTER(QTcpSocket, _socket);
    _socket->close();
    _socket->deleteLater();
    _socket = 0;
    TODO("GET(xml) if enumeration needed")
}

/*===== Configure =====*/

void VigilBehavior::enterConfiguring(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);

    VariableSet config = _plugin->configuration();
    connect_msec = config.value("ConnectMsec").toInt();
    start_msec = config.value("StartMsec").toInt();
    get_msec = config.value("GetMsec").toInt();
    maxRetry_i = config.value("MaxRetry").toInt();
    if ( ! connect_msec)    connect_msec = 5000;
    if ( ! start_msec)      start_msec = 20000;
    if ( ! get_msec)        get_msec = 5000;
    if ( ! maxRetry_i)      maxRetry_i = 5;

    TODO("Collect configuration: Resolution");
    _plugin->emitConfigured(ResultSet());
}

/*===== Start =====*/

void VigilBehavior::enterStarted(void)
{
    FUNCTION();
    OBJPOINTER(HttpManager, _manager);
    FSPPOINTER(FrameSourcePlugin, _plugin);

    QUrl url(_plugin->url());
    if ("vigilPlay" == url.scheme())
    {
        url.setScheme("http");
        url.setPath("playback");
        url.addQueryItem("cmd", "startsession");
        url.addQueryItem("camera", QString::number(_plugin->channel()));
        if ( ! _plugin->beginMst().isNull())
            url.addQueryItem("from", _plugin->beginMst().toString("yyyy-MM-dd hh:mm"));
        if ( ! _plugin->endMst().isNull())
            url.addQueryItem("to", _plugin->endMst().toString("yyyy-MM-dd hh:mm"));
        currentGet_mst = _manager->get(url, start_msec);
        TRACE("Start GET done");
        session_i = session_n = 0;
    }
    else if ("vigilLive" == url.scheme())
    {
        ResultSet rset;
        rset << Result(FrameSourceResult::Started, _plugin->url());
        _plugin->emitStarted(rset);
    }
    retry_k = 0;
    CONNECT(_manager, SIGNAL(got(MillisecondTime,bool,ResultSet)),
            _plugin, SLOT(getResult(MillisecondTime,bool,ResultSet)));
}

void VigilBehavior::handlePlayStarting(MillisecondTime mst,
                                       bool isError,
                                       ResultSet rset)
{
    FUNCTION(qint64(mst), isError, rset.size());
    RESULTS(rset);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    OBJPOINTER(HttpManager, _manager);
    ResultSet ourResult;

    if (isError)
    {
        if (rset.is(Severity::Error))
        {
            Result errRes = rset.find(HttpManager::HttpGetFailure);
            ourResult << errRes;
            _plugin->emitGrabError(ourResult);
        }
        else if (rset.is(Severity::Warning))
        {
            Result errRes = rset.find(HttpManager::HttpGetTimeout);
            ourResult << errRes;
            _plugin->emitGrabTimeout(ourResult);
        }
        else
            _plugin->emitGrabError(rset);
    }
    else
    {
        EXPECTNOT(session_i);
        EXPECTNOT(session_n);
        QString sResult;
        Result dataRes = rset.find(HttpManager::HttpGotData);
        DUMPVAR(dataRes.value("MimeType").toString());
        if ( ! dataRes.isNull())
        {
            sResult = dataRes.value("ByteArray").toByteArray();
            if (dataRes.value("MimeType").toString().contains("text/plain"))
            {
                QUrl url(dataRes.value("RequestURL").toUrl());
                TRACE("%1 from %2", sResult, url.toString());
                EXPECTNOT(sResult.isEmpty());
                EXPECTEQ(sResult.size(), dataRes.value("ExpectedBytes").toInt());
                EXPECT("playback" == url.path());
                EXPECT("startsession" == url.queryItemValue("cmd"));
                QStringList qsl(sResult.split(','));
                if (EXPECTEQ(3, qsl.size())
                        && EXPECTEQ("result:ok", qsl.at(2)))
                {
                    if (EXPECT(qsl.at(0).startsWith("session:")))
                        session_i = qsl.at(0).mid(QString("session:").size()).toInt();
                    if (EXPECT(qsl.at(1).startsWith("totalframes:")))
                        session_n = qsl.at(1).mid(QString("totalframes:").size()).toInt();
                }
            }
            else if (dataRes.value("MimeType").toString().contains("text/html"))
            {
                int x = sResult.indexOf("<body");
                sResult = sResult.mid(x + QString("<body").length());
                DUMPVAR(sResult);
                x = sResult.indexOf('>');
                sResult = sResult.mid(x + QString(">").length());
                DUMPVAR(sResult);
                x = sResult.indexOf("</body>");
                sResult.truncate(x);
                DUMPVAR(sResult);
            }
            else
            {
                WARNING("Unhandled MimeType: %1", dataRes.value("MimeType"));
            }
        }
        if (session_i && session_n)
        {
            currentFrame_i = 1;
            ourResult << Result(FrameSourceResult::Started,
                                _plugin->url(),
                                session_n);
            _plugin->emitStarted(ourResult);
        }
        else
        {
            ourResult << Result(FrameSourceResult::StartError,
                                _plugin->url(), sResult);
            _plugin->emitStartError(ourResult);
        }
    }

    EXPECTEQ(currentGet_mst, mst);
    currentGet_mst = MillisecondTime::null();
    RESULTS(ourResult);
}

void VigilBehavior::exitRunning(void)
{
    FUNCTION();
    OBJPOINTER(HttpManager, _manager);
    FSPPOINTER(FrameSourcePlugin, _plugin);

    DISCONNECT(_manager, SIGNAL(got(MillisecondTime,bool,ResultSet)),
            _plugin, SLOT(getResult(MillisecondTime,bool,ResultSet)));
    _manager->abort();

    QUrl url(_plugin->url());
    if ("vigilPlay" == url.scheme())
    {
        url.setScheme("http");
        url.setPath("playback");
        url.addQueryItem("cmd", "stopsession");
        url.addQueryItem("session", QString::number(session_i));
        _manager->get(url);
        TRACE("Stop GET done");
        currentFrame_i = session_i = session_n = 0;
    }
}

void VigilBehavior::enterFinished(void)
{
    FUNCTION();
}

/*===== Grab =====*/

bool VigilBehavior::canGrab(void)
{
    FUNCTION();
    bool b = true;
    if ( ! currentGet_mst.isNull())
        b = false;
    else if ("vigilPlay" == _plugin->url().scheme())
    {
        if (session_n < currentFrame_i)
        {
            ResultSet rset;
            rset << Result(FrameSourceResult::PlaybackComplete,
                          session_n,
                          _plugin->url(),
                          _plugin->beginMst().toString(),
                          _plugin->endMst().toString());
            _plugin->emitComplete(rset);
            b = false;
        }
    }
    FNRETURN(b);
    return b;
}

void VigilBehavior::enterGrabbing(void)
{
    FUNCTION();
    OBJPOINTER(HttpManager, _manager);
    FSPPOINTER(FrameSourcePlugin, _plugin);

    QUrl url(_plugin->url());
    if ("vigilLive" == url.scheme())
    {
        url.setScheme("http");
        url.setPath("live");
        url.addQueryItem("camera", QString::number(_plugin->channel()));
        url.addQueryItem("resolution", "VGA");
        url.addQueryItem("quality", "90");
        currentGet_mst = _manager->get(url, get_msec);
        TRACE("Grab GET done");
    }
    else if ("vigilPlay" == url.scheme())
    {
        url.setScheme("http");
        url.setPath("playback");
        url.addQueryItem("cmd", "getimage");
        url.addQueryItem("frame", QString::number(currentFrame_i));
        url.addQueryItem("session", QString::number(session_i));
        url.addQueryItem("resolution", "VGA");
        currentGet_mst = _manager->get(url, get_msec);
        TRACE("Grab GET done");
    }
    else
    {
        WARNING("Unhandled scheme: %1", url.scheme());
    }
}

void VigilBehavior::handlePlayGrab(MillisecondTime mst,
                                   bool isError,
                                   ResultSet rset)

{
    FUNCTION(qint64(mst), isError, rset.toStringList());
    RESULTS(rset);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    OBJPOINTER(HttpManager, _manager);
    ResultSet ourResult;

    if (isError)
    {
        if (rset.is(Severity::Error))
        {
            Result errRes = rset.find(HttpManager::HttpGetFailure);
            ourResult << errRes;
            _plugin->emitGrabError(ourResult);
        }
        else if (rset.is(Severity::Warning))
        {
            Result errRes = rset.find(HttpManager::HttpGetTimeout);
            ourResult << errRes;
            _plugin->emitGrabTimeout(ourResult);
        }
        else
            _plugin->emitGrabError(rset);
    }
    else
    {
        Result dataRes = rset.find(HttpManager::HttpGotData);
        if ( ! dataRes.isNull())
        {
            QByteArray ba(dataRes.value("ByteArray").toByteArray());
            EXPECTNOT(ba.isEmpty());
            EXPECTEQ(ba.size(), dataRes.value("ExpectedBytes").toInt());
            DUMPVAR(dataRes.value("MimeType").toString());
            if (EXPECT(dataRes.value("MimeType").toString().contains("image/jpeg")))
            {
                QString FrameId = QString("F%1").arg(currentFrame_i, 7, 10, QChar('0'));
                ImageEntity frameEntity
                        = ImageEntity::fromByteArray(ba,
                                                     currentGet_mst,
                                                     FrameId);
                _plugin->enqueueGrab(frameEntity);
                Result goodRes = rset.find(HttpManager::HttpGetSuccess);
                EXPECTNOT(goodRes.isNull());
                ourResult << goodRes;
                _plugin->emitGrabbed(ourResult);
                ++currentFrame_i;
                if (retry_k > 0) --retry_k;
            }
            else if (dataRes.value("MimeType").toString().contains("text/plain"))
            {
                QString sResult = dataRes.value("ByteArray").toString();
                QUrl url(dataRes.value("RequestURL").toUrl());
                TRACE("%1 from %2", sResult, url.toString());
                EXPECTEQ(sResult.size(), dataRes.value("ExpectedBytes").toInt());
                ourResult << Result(HttpManager::HttpGetFailure, -1, sResult);
                _plugin->emitGrabError(ourResult);
            }
            else if (dataRes.value("MimeType").toString().contains("text/html"))
            {
                QString sResult = dataRes.value("ByteArray").toString();
                int x = sResult.indexOf("<body");
                sResult = sResult.mid(x + QString("<body").length());
                DUMPVAR(sResult);
                x = sResult.indexOf('>');
                sResult = sResult.mid(x + QString(">").length());
                DUMPVAR(sResult);
                x = sResult.indexOf("</body>");
                sResult.truncate(x);
                DUMPVAR(sResult);
                ourResult << Result(HttpManager::HttpGetFailure, -2, sResult);
                _plugin->emitGrabError(ourResult);
            }
            else
            {
                WARNING("Unhandled MimeType: %1", dataRes.value("MimeType"));
            }
        }
    }

    RESULTS(ourResult);
    EXPECTEQ(currentGet_mst, mst);
    currentGet_mst = MillisecondTime::null();
}

void VigilBehavior::handleLiveGrab(MillisecondTime mst,
                                   bool isError,
                                   ResultSet rset)

{
    FUNCTION(qint64(mst), isError, rset.toStringList());
    RESULTS(rset);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    OBJPOINTER(HttpManager, _manager);
    ResultSet ourResult;

    if (isError)
    {
        if (rset.is(Severity::Error))
        {
            Result errRes = rset.find(HttpManager::HttpGetFailure);
            ourResult << errRes;
            _plugin->emitGrabError(ourResult);
        }
        else if (rset.is(Severity::Warning))
        {
            Result errRes = rset.find(HttpManager::HttpGetTimeout);
            ourResult << errRes;
            _plugin->emitGrabTimeout(ourResult);
        }
        else
            _plugin->emitGrabError(rset);
    }
    else
    {
        Result dataRes = rset.find(HttpManager::HttpGotData);
        if ( ! dataRes.isNull())
        {
            QByteArray ba(dataRes.value("ByteArray").toByteArray());
            EXPECTNOT(ba.isEmpty());
            EXPECTEQ(ba.size(), dataRes.value("ExpectedBytes").toInt());
            DUMPVAR(dataRes.value("MimeType").toString());
            if (EXPECT(dataRes.value("MimeType").toString().contains("image/jpeg")))
            {
                ImageEntity frameEntity
                        = ImageEntity::fromByteArray(ba, currentGet_mst);
                _plugin->enqueueGrab(frameEntity);
                Result goodRes = rset.find(HttpManager::HttpGetSuccess);
                EXPECTNOT(goodRes.isNull());
                ourResult << goodRes;
                RESULTS(ourResult);
                _plugin->emitGrabbed(ourResult);
                if (retry_k > 0) --retry_k;
            }
            else if (dataRes.value("MimeType").toString().contains("text/plain"))
            {
                QString sResult = dataRes.value("ByteArray").toString();
                QUrl url(dataRes.value("RequestURL").toUrl());
                TRACE("%1 from %2", sResult, url.toString());
                EXPECTEQ(sResult.size(), dataRes.value("ExpectedBytes").toInt());
                ourResult << Result(HttpManager::HttpGetFailure, -1, sResult);
                _plugin->emitGrabError(ourResult);
            }
            else if (dataRes.value("MimeType").toString().contains("text/html"))
            {
                QString sResult = dataRes.value("ByteArray").toString();
                int x = sResult.indexOf("<body");
                sResult = sResult.mid(x + QString("<body").length());
                DUMPVAR(sResult);
                x = sResult.indexOf('>');
                sResult = sResult.mid(x + QString(">").length());
                DUMPVAR(sResult);
                x = sResult.indexOf("</body>");
                sResult.truncate(x);
                DUMPVAR(sResult);
                ourResult << Result(HttpManager::HttpGetFailure, -2, sResult);
                _plugin->emitGrabError(ourResult);
            }
            else
            {
                WARNING("Unhandled MimeType: %1", dataRes.value("MimeType"));
            }
        }
    }

    RESULTS(ourResult);
    EXPECTEQ(currentGet_mst, mst);
    currentGet_mst = MillisecondTime::null();
}

void VigilBehavior::enterRetry(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);

    if (++retry_k > maxRetry_i)
    {
        ResultSet rset;
        rset << Result(HttpManager::HttpGetFailure,
                       -retry_k, "Excessive Timeout");
        _plugin->emitGrabError(rset);
    }
    DUMPVAR(retry_k);
}
