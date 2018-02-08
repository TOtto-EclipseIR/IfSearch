#ifndef HTTPBEHAVIOR_H
#define HTTPBEHAVIOR_H
#include "eirFrameSource.h"

#include "../eirTypes/MillisecondTime.h"

#include "FrameSourceBehavior.h"

class QNetworkReply;
class QTcpSocket;

class ResultSet;
class HttpManager;

class EIRFRAMESOURCESHARED_EXPORT HttpBehavior
        : public FrameSourceBehavior
{
public:
    HttpBehavior(void);
    FrameSourceBehavior * brother(void);
    QString name(void) const;
    QStringList schemeList(void);
    bool supports(const QUrl & url);
    bool supportsScheme(const QString & scheme);
    bool canGrab(void);

    void enterConstruct(void);
    void enterConfiguring(void);
    void enterConnecting(void);
    void exitConnecting(void);
    void enterGrabbing(void);

    void connectResult(void);
    void getResult(MillisecondTime mst,
                   bool isError,
                   ResultSet rset);

private:
    QTcpSocket * _socket;
    HttpManager * _manager;
    MillisecondTime currentGet_mst;
};

#endif // HTTPBEHAVIOR_H
