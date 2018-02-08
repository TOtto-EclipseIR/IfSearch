#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include "eirNetwork.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QUrl>

#include "../eirBase/DataProperty.h"
#include "../eirTypes/MillisecondDelta.h"
#include "../eirTypes/MillisecondTime.h"

#define HTTPREQUEST_DATAPROPS(TND) \
    TND(MillisecondTime, RequestMst, MillisecondTime()) \
    TND(MillisecondDelta, TimeoutMsd, MillisecondDelta()) \
    TND(MillisecondTime, CompleteMst, MillisecondTime()) \
    TND(QUrl, Url, QUrl()) \
    TND(qint64, Count, 0) \
    TND(qint64, Total, 0) \
    TND(int, NetworkError, 0) \

class HttpRequestData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(HTTPREQUEST_DATAPROPS);
public:
    HttpRequestData(void)
    {
        DEFINE_DATAPROPS_CTORS(HTTPREQUEST_DATAPROPS);
    }
};

class EIRNETWORKSHARED_EXPORT HttpRequest
{
    DECLARE_PARENT_DATAPROPS(HTTPREQUEST_DATAPROPS);
public:
    HttpRequest(void); // default c'tor
    HttpRequest(const HttpRequest &);
    HttpRequest &operator=(const HttpRequest &);
    ~HttpRequest();

private:
    QSharedDataPointer<HttpRequestData> data;
};

#endif // HTTPREQUEST_H
