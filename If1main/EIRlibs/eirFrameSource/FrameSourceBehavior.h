#ifndef FRAMESOURCEBEHAVIORBASE_H
#define FRAMESOURCEBEHAVIORBASE_H
#include "eirFrameSource.h"


class QStringList;
class QUrl;

#include "../eirBase/VersionInfo.h"

class MillisecondTime;
class ResultSet;
class FrameSourcePlugin;
class VersionInfo;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceBehavior
{
protected:
    FrameSourceBehavior(void);

public:
    void setPlugin(FrameSourcePlugin * plugin);
    virtual FrameSourceBehavior * brother(void);
    virtual QString name(void) const;
    virtual QStringList schemeList(void);
    virtual bool supports(const QUrl & url);
    virtual bool supportsScheme(const QString & scheme);
    virtual bool canGrab(void);
    virtual void preGrab(void);
    VersionInfo version(void) const;
    void setVersion(const VersionInfo & vi);

    virtual void enterConstruct(void);
    virtual void enterConnecting(void);
    virtual void enterConnected(void);
    virtual void enterConfiguring(void);
    virtual void enterConfigured(void);
    virtual void enterStarted(void);
    virtual void enterRunning(void);
    virtual void enterReady(void);
    virtual void enterGrabbing(void);
    virtual void enterRetry(void);
    virtual void enterPaused(void);
    virtual void enterFinished(void);
    virtual void enterError(void);
    virtual void enterFinal(void);

    virtual void exitConstruct(void);
    virtual void exitConnecting(void);
    virtual void exitConnected(void);
    virtual void exitConfiguring(void);
    virtual void exitConfigured(void);
    virtual void exitStarted(void);
    virtual void exitRunning(void);
    virtual void exitReady(void);
    virtual void exitGrabbing(void);
    virtual void exitRetry(void);
    virtual void exitPaused(void);
    virtual void exitFinished(void);
    virtual void exitError(void);
    virtual void exitFinal(void);

    virtual void connectResult(void);
    virtual void getResult(MillisecondTime mst,
                           bool isError,
                           ResultSet rset);


protected:
    FrameSourcePlugin * _plugin;
    VersionInfo _vi;
};

#endif // FRAMESOURCEBEHAVIORBASE_H
