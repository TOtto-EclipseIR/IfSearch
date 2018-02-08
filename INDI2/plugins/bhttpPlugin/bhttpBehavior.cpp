#include "bhttpBehavior.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>

#include <eirExe/Log.h>
#include <eirExe/ResultId.h>
#include <eirNetwork/HttpManager.h>
#include <eirFrameSource/FrameSourceResult.h>

#include "bhttpPlugin.h"

bhttpBehavior::bhttpBehavior(void)
    : _socket(0)
    , _manager(0)
    , _reply(0)
    , currentGet_mst(MillisecondTime::current())
    , offset_i(0)
{
}

FrameSourceBehavior * bhttpBehavior::brother(void)
{
    return new bhttpBehavior();
}

QString bhttpBehavior::name(void) const
{
    return "bhttp";
}

QStringList bhttpBehavior::schemeList(void)
{
    FUNCTION();
    FNRETURN("bhttp");
    return QStringList() << "bhttp";
}

bool bhttpBehavior::supports(const QUrl & url)
{
    FUNCTION();
    bool b = ("bhttp" == url.scheme());
    FNRETURN(b);
    return b;
}

bool bhttpBehavior::supportsScheme(const QString & scheme)
{
    FUNCTION();
    bool b = ("bhttp" == scheme);
    FNRETURN(b);
    return b;
}

/*===== Construct =====*/

void bhttpBehavior::enterConstruct(void)
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
    CONNECT(_manager, SIGNAL(got(MillisecondTime,bool,ResultSet)),
            _plugin, SLOT(getResult(MillisecondTime,bool,ResultSet)));
    ResultSet rset;
    rset << Result(FrameSourceResult::Initialized, name());
    _plugin->emitInitialized(rset);
}

/*===== Configure =====*/

void bhttpBehavior::enterConfiguring(void)
{
    FUNCTION();
    TODO("Collect configuration");
    _plugin->emitConfigured(ResultSet());
}

/*===== Connect =====*/

void bhttpBehavior::enterConnecting(void)
{
    FUNCTION();
    OBJPOINTER(QTcpSocket, _socket);
    _socket->connectToHost(_plugin->url().host(),
                           _plugin->url().port(80));
    TRACE("connectToHost(%1, %2)", _plugin->url().host(),
          _plugin->url().port(80));
}

void bhttpBehavior::connectResult(void)
{
    FUNCTION();
    OBJPOINTER(QTcpSocket, _socket);
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
                       _plugin->url().toString(),
                       _socket->state(),
                       _socket->error(),
                       _socket->errorString()));
        _plugin->emitConnectError(rset);
        _socket->abort();
    }

    RESULTS(rset);
}

void bhttpBehavior::exitConnecting(void)
{
    FUNCTION();
    _socket->close();
    _socket->deleteLater();
    _socket = 0;
    currentGet_mst = MillisecondTime::null();
}

/*===== Grab =====*/

bool bhttpBehavior::canGrab(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    bool b = true;
    if ( ! currentGet_mst.isNull())     b = false;
    else if (_plugin->isPaused())       b = false;
    FNRETURN(b);
    return b;
}

void bhttpBehavior::enterGrabbing(void)
{
    FUNCTION();
    OBJPOINTER(HttpManager, _manager);
    FSPPOINTER(FrameSourcePlugin, _plugin);

    QUrl url(_plugin->url());
    url.setScheme("http");
    currentGet_mst = _manager->get(url);
    TRACE("Grab GET done");
}

void bhttpBehavior::getResult(MillisecondTime mst,
                              bool isError,
                              ResultSet rset)
{
    FUNCTION(qint64(mst), isError, rset.size());
    RESULTS(rset);
    FSPPOINTER(FrameSourcePlugin, _plugin);
    OBJPOINTER(HttpManager, _manager);
    ResultSet ourResult;

    EXPECTEQ(currentGet_mst, mst);

    if (isError)
    {
        Result errRes = rset.find(FrameSourceResult::HttpGetFailure);
        ourResult << errRes;
        RESULTS(ourResult);
        _plugin->emitGrabError(ourResult);
    }
    else
    {
        Result dataRes = rset.find(HttpManager::HttpGotData);
        if ( ! dataRes.isNull())
        {
            QByteArray ba(dataRes.value("ByteArray").toByteArray());
            EXPECTNOT(ba.isEmpty());
            EXPECTEQ(ba.size(), dataRes.value("ExpectedBytes").toInt());
            EXPECT(dataRes.value("MimeType")
                   .toString().contains("image/jpeg"));
            ImageEntity frameEntity
                    = ImageEntity::fromByteArray(ba, currentGet_mst);
            _plugin->enqueueGrab(frameEntity);
        }
        Result goodRes = rset.find(FrameSourceResult::HttpGetSuccess);
        EXPECTNOT(goodRes.isNull());
        ourResult << goodRes;
        RESULTS(ourResult);
        _plugin->emitGrabbed(ourResult);
    }

    currentGet_mst = MillisecondTime::null();
}
