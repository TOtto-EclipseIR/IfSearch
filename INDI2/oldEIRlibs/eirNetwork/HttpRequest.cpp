#include "HttpRequest.h"
#include <QSharedData>

HttpRequest::HttpRequest() : data(new HttpRequestData)
{
}

HttpRequest::HttpRequest(const HttpRequest &rhs) : data(rhs.data)
{
}

HttpRequest &HttpRequest::operator=(const HttpRequest &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

HttpRequest::~HttpRequest()
{
}

