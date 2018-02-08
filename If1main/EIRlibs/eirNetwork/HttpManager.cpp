#include "HttpManager.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QTimer>

#include "../eirExe/Log.h"
#include "../eirExe/ResultId.h"
#include "../eirExe/ResultInfo.h"

const ResultCode HttpManager::HttpGetSuccess("eNwHGrab");
const ResultCode HttpManager::HttpGetTimeout("eNwHGTmO");
const ResultCode HttpManager::HttpGetFailure("eNwHGrbE");
const ResultCode HttpManager::HttpGotData("eNwGotDa");

HttpManager::HttpManager(QObject * parent)
    : QObject(parent)
    , _manager(new QNetworkAccessManager(this))
    , second_timer(new QTimer(this))
    , maxActive_i(6)
    , maxComplete_i(256)
{
    FUNCTION();
    OBJPOINTER(QNetworkAccessManager, _manager);
    OBJPOINTER(QTimer, second_timer);
    setObjectName("HttpManager");

    second_timer->setInterval(1000);
    second_timer->setSingleShot(false);
    CONNECT(second_timer, SIGNAL(timeout()),
            this, SLOT(pulse()));
}

MillisecondTime HttpManager::get(const QUrl & url,
                    const MillisecondDelta timeout)
{
    FUNCTION(url.toString(), qint64(timeout));
    if (get_reply_request_map.size() >= maxActive_i)
        return MillisecondTime();
    if ( ! second_timer->isActive())    second_timer->start();

    QNetworkRequest request(url);
    MillisecondTime mst(MillisecondTime::current());
    QNetworkReply * reply = _manager->get(request);
    CONNECT(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(progress(qint64,qint64)));
    CONNECT(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(error(QNetworkReply::NetworkError)));

    HttpRequest rq;
    rq.setRequestMst(mst);
    rq.setTimeoutMsd((timeout < 1000)
                     ? MillisecondDelta()
                     : timeout);
    rq.setUrl(url);
    get_reply_request_map.insert(reply, rq);

    FNRETURN(qint64(mst));
    return mst;
}

void HttpManager::get(QList<QUrl> urls,
                    const MillisecondDelta timeout)
{
    FUNCTION(urls.size(), qint64(timeout));

    foreach (QUrl url, urls)
    {
        HttpRequest rq;
        rq.setUrl(url);
        rq.setTimeoutMsd(timeout);
        get_request_list.append(rq);
    }
    if ( ! second_timer->isActive())    second_timer->start();
}

void HttpManager::progress(qint64 count, qint64 total)
{
    FUNCTION(count, total);

    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    OBJPOINTER(QNetworkReply, reply);

    MillisecondTime mst = get_reply_request_map[reply].getRequestMst();
    EMIT(progress(mst, count, total));
    get_reply_request_map[reply].setCount(count);
    get_reply_request_map[reply].setTotal(total);

    if (total > 0LL && count == total)
    {
        // Request is finished
        get_reply_request_map[reply]
                .setCompleteMst(MillisecondTime::current());
        QUrl url = get_reply_request_map[reply].getUrl();
        QByteArray ba(reply->readAll());
        ResultSet rset;
        rset << Result(HttpGetSuccess, ba.size(), url);

        QVariantList qvl;
        qvl << url;
        qvl << ba;
        qvl << reply->header(QNetworkRequest::ContentLengthHeader);
        qvl << reply->header(QNetworkRequest::ContentTypeHeader);
        qvl << reply->header(QNetworkRequest::LocationHeader);
        qvl << reply->header(QNetworkRequest::LastModifiedHeader);
        rset << Result(HttpGotData, qvl);
        RESULTS(rset);
        EMIT(got(mst, false, rset));
    }
}

void HttpManager::abort(void)
{
    foreach (QNetworkReply * reply,
             get_reply_request_map.keys())
    {
        OBJPOINTER(QNetworkReply, reply);
        reply->abort();
    }
}

void HttpManager::error(QNetworkReply::NetworkError errorCode)
{
    FUNCTION(errorCode);

    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    OBJPOINTER(QNetworkReply, reply);

    if (QNetworkReply::OperationCanceledError == errorCode) // 5
    {
        get_reply_request_map.remove(reply);
    }
    else if (get_reply_request_map[reply].getCompleteMst().isNull())
    {
        ResultSet rset;
        MillisecondTime mst = get_reply_request_map[reply].getRequestMst();
        get_reply_request_map[reply].setNetworkError(errorCode);
        get_reply_request_map[reply]
                .setCompleteMst(MillisecondTime::current());
        rset << Result(HttpGetFailure,
                      errorCode,
                      reply->errorString(),
                      get_reply_request_map[reply].getUrl());
        EMIT(got(mst, true, rset));
    }
    else
    {
        TRACE("Likely duplicate error() call");
    }
}

void HttpManager::timeout(QNetworkReply * reply,
                            HttpRequest & rq)
{
    OBJPOINTER(QNetworkReply, reply);
    FUNCTION(OBJNAME(reply), rq.getUrl());

    MillisecondTime mst = get_reply_request_map[reply].getRequestMst();
    get_reply_request_map[reply].setNetworkError( - mst.delta());
    get_reply_request_map[reply]
            .setCompleteMst(MillisecondTime::current());

    ResultSet rset;
    rset << Result(HttpGetTimeout,
                  get_reply_request_map[reply].getNetworkError(),
                  QString("Timeout"),
                  get_reply_request_map[reply].getUrl());
    EMIT(got(mst, true, rset));
    reply->close();
}

void HttpManager::clearHistory(void)
{
    TODO("Do something with stats");
    complete_request_list.clear();
}

void HttpManager::pulse(void)
{
    FUNCTION();
    OBJPOINTER(QNetworkAccessManager, _manager);
    clearHistory();

    foreach(QNetworkReply * reply,
            get_reply_request_map.keys())
    {
        OBJPOINTER(QNetworkReply, reply)
        HttpRequest rq = get_reply_request_map.value(reply);
        if ( ! rq.getCompleteMst().isNull())
        {   // request completed, remove from map
            get_reply_request_map.remove(reply);
            complete_request_list.append(rq);
            calculate(rq);
            reply->deleteLater();
        }
        else if (rq.getTimeoutMsd()
                    && rq.getRequestMst().delta()> rq.getTimeoutMsd())
        {   // request timed out
            timeout(reply, rq);
        }
    }

    while (get_reply_request_map.size() < maxActive_i
           && ! get_request_list.isEmpty())
    {
        HttpRequest rq = get_request_list.takeFirst();
        QNetworkRequest qnrq(rq.getUrl());
        MillisecondTime mst = MillisecondTime::current();
        QNetworkReply * reply = _manager->get(qnrq);
        rq.setRequestMst(mst);
        get_reply_request_map.insert(reply, rq);
    }

    if (complete_request_list.size() > maxComplete_i)
        EMIT(statsFull(complete_request_list.size()));
    int pad = maxComplete_i >> 2;
    if (complete_request_list.size() > maxComplete_i + pad)
        while (complete_request_list.size() > maxComplete_i)
            complete_request_list.removeFirst();

    DUMPVAR(get_reply_request_map.size());
    if (get_reply_request_map.isEmpty()
            && get_request_list.isEmpty()
            && second_timer->isActive())
        second_timer->stop();
}

void HttpManager::calculate(const HttpRequest & rq)
{
    FUNCTION();
    (void)rq;
    TODO("calculate");
}


const ResultInfo ri01(HttpManager::HttpGotData, Severity::Data,
                    ResultId("eir", "Network","Http","GotData"),
                    QString(),
                    "Data received from HTTP GET command.",
                    QStringList()
                      << "RequestURL"
                      << "ByteArray"
                      << "ExpectedBytes"
                      << "MimeType"
                      << "ActualURL"
                      << "ModifiedDtTm"
                      );

const ResultInfo ri06(HttpManager::HttpGetSuccess, Severity::Progress,
                 ResultId("eir", "FrameSource","Http","GrabSuccess"),
                 "%1-byte frame grabbed from %2.",
                 "Frame was received from the specified URL",
                  QStringList() << "FrameBytes"
                  << "URL");

const ResultInfo ri12(HttpManager::HttpGetTimeout, Severity::Warning,
                  ResultId("eir", "FrameSource","Http","GrabTimeout"),
                 "HTTP GET timeout=%1 %2",
                 "The frame get from the specified URL did not respond"
                 "We will try again.",
                  QStringList() << "ErrorCode" << "ErrorString");

const ResultInfo ri07(HttpManager::HttpGetFailure, Severity::Error,
                  ResultId("eir", "FrameSource","Http","GrabFailure"),
                 "HTTP GET error=%1 %2",
                 "The frame get from the specified URL resulted in an error. "
                 "Please reconnect.",
                  QStringList() << "ErrorCode" << "ErrorString");

