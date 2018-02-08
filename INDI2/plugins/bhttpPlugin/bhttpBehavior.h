#ifndef BHTTPBEHAVIOR_H
#define BHTTPBEHAVIOR_H
#include "bhttpPlugin.h"

#include <eirTypes/MillisecondTime.h>
#include <eirFrameSource/FrameSourceBehavior.h>

class QNetworkReply;
class QTcpSocket;

class HttpManager;

class BHTTPPLUGINSHARED_EXPORT bhttpBehavior
        : public FrameSourceBehavior
{
public:
    bhttpBehavior(void);
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
    QNetworkReply * _reply;
    MillisecondTime currentGet_mst;
    QByteArray currentGet_ba;
    int offset_i;
};

#endif // BHTTPBEHAVIOR_H
