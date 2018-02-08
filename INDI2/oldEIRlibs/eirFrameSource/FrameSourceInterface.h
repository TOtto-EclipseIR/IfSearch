#ifndef FRAMESEQUENCEINTERFACE_H
#define FRAMESEQUENCEINTERFACE_H
#include "eirFrameSource.h"

#include <QtPlugin>

#include <QtCore/QUrl>

class QDomElement;
class QString;
class QStringList;

class Severity;
class VariableSet;
class ResultSet;
class ImageEntity;

class FrameSourceInterface
{
public:
    virtual ~FrameSourceInterface() {}

    virtual QStringList schemeList(void) = 0;
    virtual bool supports(const QUrl & url) = 0;
    virtual bool supportsScheme(const QString & scheme) = 0;

    virtual void setUrl(const QUrl & url=QUrl()) = 0;
    virtual void connect(void) = 0;
    virtual void setResultSeverity(const Severity sev) = 0;
    virtual void configure(const VariableSet & config) = 0;
    virtual void start(const int channel=-1,
                       const QString & begin=QString(),
                       const QString & end=QString()) = 0;
    virtual void pause(void) = 0;
    virtual void resume(void) = 0;
    virtual void abort(void) = 0;
    virtual void stop(void) = 0;
    virtual void shutdown(void) = 0;

    virtual bool canGrab(void) = 0;
    virtual void grab(void) = 0;
    virtual bool canTake(void) const = 0;
    virtual ImageEntity take(void) = 0;

    virtual VariableSet configuration(void) const = 0;
    virtual ResultSet status(void) const = 0;
    virtual ResultSet results(void) const = 0;
    virtual ResultSet takeResults(void) = 0;

    virtual bool isInitialized(void) const = 0;
    virtual bool isConfigured(void) const = 0;
    virtual bool isStarted(void) const = 0;
    virtual bool isPaused(void) const = 0;
    virtual bool isFinished(void) const = 0;

    virtual QDomElement enumerate(const int channel=-1) = 0;
    virtual QString channelId(const int channel=-1) = 0;
};
Q_DECLARE_INTERFACE(FrameSourceInterface, "EclipseIR.com/FrameSource")

#endif // FRAMESEQUENCEINTERFACE_H
