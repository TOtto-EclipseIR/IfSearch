#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H
#include "eirNetwork.h"

#include <QtCore/QObject>

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include "../eirTypes/MillisecondTime.h"
#include "../eirTypes/MillisecondDelta.h"
#include "../eirExe/ResultCode.h"
#include "../eirExe/Results.h"

#include "HttpRequest.h"

class QDomDocument;
class QNetworkAccessManager;
class QTimer;

class VariableTable;

class EIRNETWORKSHARED_EXPORT HttpManager : public QObject
{
    Q_OBJECT

public:
    explicit HttpManager(QObject * parent=0);
    MillisecondTime get(const QUrl & url,
                        const MillisecondDelta timeout=10000);
    void get(QList<QUrl> urls,
                        const MillisecondDelta timeout=10000);
    QDomDocument statsReport(void) const;
    VariableTable takeHistory(void);

public:
    const static ResultCode HttpGotData;
    const static ResultCode HttpGetSuccess;
    const static ResultCode HttpGetTimeout;
    const static ResultCode HttpGetFailure;

public slots:
    void progress(qint64 count, qint64 total);
    void error(QNetworkReply::NetworkError errorCode);
    void abort(void);
    void clearHistory(void);

signals:
    void got(MillisecondTime mst, bool error, ResultSet rset);
    void progress(MillisecondTime mst, qint64 count, quint64 total);
    void statsFull(int size);

private:
    void calculate(const HttpRequest & rq);
    void timeout(QNetworkReply * reply,
                 HttpRequest & rq);

private slots:
    void pulse(void);

private:
    QNetworkAccessManager * _manager;
    QTimer * second_timer;
    int maxActive_i;
    int maxComplete_i;
    QMap<QNetworkReply *, HttpRequest> get_reply_request_map;
    QList<HttpRequest> get_request_list;
    QList<HttpRequest> complete_request_list;
};

#endif // HTTPMANAGER_H
