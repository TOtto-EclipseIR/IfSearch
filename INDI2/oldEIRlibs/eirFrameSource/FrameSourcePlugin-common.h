#ifndef FRAMESOURCEPLUGINCOMMON_H
#define FRAMESOURCEPLUGINCOMMON_H

bool isNull(void) const;
QStringList schemeList(void);
bool supports(const QUrl & url);
bool supportsScheme(const QString & scheme);
VersionInfo version(void) const;

void startMachine(void);
void setBehavior(FrameSourceBehavior * beh);
virtual void setResultSeverity(const Severity sev);

void setUrl(const QUrl & url=QUrl());
void connect(void);
void configure(const VariableSet & config);
void start(const int channel=-1,
           const QString & begin=QString(),
           const QString & end=QString());
void pause(void);
void resume(void);
void abort(void);
void stop(void);
void shutdown(void);

bool canGrab(void);
void grab(void);
bool canTake(void) const;
ImageEntity take(void);

VariableSet configuration(void) const;
virtual ResultSet results(void) const;
virtual ResultSet takeResults(void);
int channel(void) const;
MillisecondTime beginMst(void) const;
MillisecondTime endMst(void) const;

Result status(const int x) const;
ResultSet status(void) const;
bool isInitialized(void) const;
bool isConfigured(void) const;
bool isStarted(void) const;
bool isPaused(void) const;
bool isFinished(void) const;

virtual QDomElement enumerate(const int channel=-1);
virtual QString channelId(const int channel=-1);

FrameSourceState state(void) const;
QString stateName(void) const;
QUrl url(void) const;

void emitInitialized(ResultSet rset);
void emitConnected(ResultSet rset);
void emitConnectError(ResultSet rset);
void emitConfigured(ResultSet rset);
void emitStarted(ResultSet rset);
void emitStartError(ResultSet rset);
void emitGrabbed(ResultSet rset);
void emitGrabTimeout(ResultSet rset);
void emitGrabError(ResultSet rset);
void emitComplete(ResultSet rset);

void enqueueGrab(const ImageEntity & ie);

signals:
void onInitialize(void);
void onConnect(void);
void onConfigure(void);
void onStart(void);
void onPause(void);
void onResume(void);
void onAbort(void);
void onStop(void);
void onShutdown(void);
void doGrab(void);
void progress(MillisecondTime mst,
                  int count,
                  int total);

void initialized(ResultSet rset);
void configured(ResultSet rset);
void started(ResultSet rset);
void startError(ResultSet rset);
void connected(ResultSet rset);
void connectError(ResultSet rset);
void stopped(ResultSet rset);
void paused(ResultSet rset);
void resumed(ResultSet rset);
void aborted(ResultSet rset);
void complete(ResultSet rset);
void shutdowned(ResultSet rset);
void grabbed(ResultSet rset);
void grabTimeout(ResultSet rset);
void grabError(ResultSet rset);
void taken(ImageEntity img);
void initialized(void);
void configured(void);
void started(void);
void startError(void);
void connected(void);
void connectError(void);
void complete(void);
void stopped(void);
void paused(void);
void resumed(void);
void aborted(void);
void shutdowned(void);
void grabbed(void);
void grabTimeout(void);
void grabError(void);
void taken(void);

public slots:
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
void httpProgress(MillisecondTime mst,
                  int count,
                  int total);

private:
void setStatus(const FrameSourceStatusIndex x,
               const Result & r);

private:
FrameSourceBehavior * _behavior;
FrameSourceMachine * _machine;
HttpManager * _httpManager;
QUrl _url;
VariableSet config_vars;
VariableSet channel_vars;
MillisecondTime begin_mst;
MillisecondTime end_mst;
int channel_i;
ResultSet running_results;
ResultSet status_results;
QList<ImageEntity> frame_image_list;
int maxQueue_i;

#endif // FRAMESOURCEPLUGINCOMMON_H
