#ifndef VIGILBEHAVIOR_H
#define VIGILBEHAVIOR_H
#include "vigil_global.h"

#include <eirTypes/MillisecondTime.h>
#include <eirFrameSource/FrameSourceBehavior.h>

class QNetworkReply;
class QTcpSocket;

class HttpManager;

class VIGILSHARED_EXPORT VigilBehavior
        : public FrameSourceBehavior
{
public:
    VigilBehavior(void);
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
    void enterStarted(void);
    void enterGrabbing(void);
    void enterRetry(void);
    void exitRunning(void);
    void enterFinished(void);

    void connectResult(void);
    void getResult(MillisecondTime mst,
                   bool isError,
                   ResultSet rset);

private:
    void handleLiveGrab(MillisecondTime mst,
               bool isError,
               ResultSet rset);
    void handlePlayGrab(MillisecondTime mst,
               bool isError,
               ResultSet rset);
    void handlePlayStarting(MillisecondTime mst,
               bool isError,
               ResultSet rset);

private:
    QTcpSocket * _socket;
    HttpManager * _manager;
    QNetworkReply * _reply;
    MillisecondTime currentGet_mst;
    int currentFrame_i;
    int session_i;
    int session_n;
    int retry_k;
    int connect_msec;
    int start_msec;
    int get_msec;
    int maxRetry_i;
    QList<QUrl> play_url_list;
};


#endif // VIGILBEHAVIOR_H

