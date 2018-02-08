#ifndef FRAMESOURCEBASE_H
#define FRAMESOURCEBASE_H
#include "eirFrameSource.h"

#include <QObject>
#include "FrameSourceInterface.h"

#include "../eirExe/Results.h"
#include "../eirImage/ImageEntity.h"
#include "../eirImage/MessageImageGenerator.h"

class VariableSet;

class QUrl;
class FrameSourcePlugin;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceInterfaceBase
        : public QObject, FrameSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(FrameSourceInterface)

public:
    explicit FrameSourceInterfaceBase(FrameSourcePlugin * parent=0);
    virtual QStringList schemeList(void);
    virtual bool supports(const QUrl & url);
    virtual bool supportsScheme(const QString & scheme);

    virtual void setUrl(const QUrl & url=QUrl());
    virtual void connect(void);
    virtual void setResultSeverity(const Severity sev);
    virtual void configure(const VariableSet & config);
    virtual void start(const int channel=-1,
                       const QString & begin=QString(),
                       const QString & end=QString());
    virtual void pause(void);
    virtual void resume(void);
    virtual void abort(void);
    virtual void stop(void);
    virtual void shutdown(void);

    virtual bool canGrab(void);
    virtual void grab(void);
    virtual bool canTake(void) const;
    virtual ImageEntity take(void);

    virtual VariableSet configuration(void) const;
    virtual ResultSet status(void) const;
    virtual ResultSet results(void) const;
    virtual ResultSet takeResults(void);

    virtual bool isInitialized(void) const;
    virtual bool isConfigured(void) const;
    virtual bool isStarted(void) const;
    virtual bool isPaused(void) const;
    virtual bool isFinished(void) const;

    virtual QDomElement enumerate(const int channel=-1);
    virtual QString channelId(const int channel=-1);

    void autoStart(const QUrl & url,
                   const VariableSet & config,
                   const int channel=-1,
                   const QString & begin=QString(),
                   const QString & end=QString());
    ImageEntity take(int frameKey);

signals:
    void initialized(ResultSet rset);
    void connected(ResultSet rset);
    void configured(ResultSet rset);
    void started(ResultSet rset);
    void stopped(ResultSet rset);
    void grabbed(ResultSet rset);
    void complete(ResultSet rset);
    void messageImage(ImageEntity ie);
    void warning(Result res);

public slots:
    void pluginInitialized(ResultSet rset);
    void doConnect(void);
    void pluginConnected(ResultSet rset);
    void doConfigure(void);
    void pluginConfigured(ResultSet rset);
    void doStart(void);
    void pluginStarted(ResultSet rset);
    void pluginStopped(ResultSet rset);
    void pluginGrabbed(ResultSet rset);
    void pluginComplete(ResultSet rset);

protected:
    qint64 enqueue(ImageEntity frameEntity);

protected:
    FrameSourcePlugin * _plugin;
    QMap<qint64, ImageEntity> key_entity_map;
    MessageImageGenerator _miGenerator;
    bool miEnable_b;
    bool autoStart_b;
    QUrl auto_url;
    VariableSet autoConfig_vars;
    int autoChannel_i;
    QString autoBegin_s;
    QString autoEnd_s;
};

#endif // FRAMESOURCEBASE_H
