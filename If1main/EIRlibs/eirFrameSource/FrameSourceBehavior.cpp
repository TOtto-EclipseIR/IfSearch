#include "FrameSourceBehavior.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "version.h"
#include "../../../common/version.h"

#include "../eirExe/Log.h"

#include "FrameSourcePlugin.h"

FrameSourceBehavior::FrameSourceBehavior(void) : _plugin(0) { SETVERSION(); }
FrameSourceBehavior * FrameSourceBehavior::brother(void) { return 0; }
QString FrameSourceBehavior::name(void) const { return "FrameSourceBehavior"; }
bool FrameSourceBehavior::supports(const QUrl & url) { FUNCTION(); FNRETURN(false); return false; }
QStringList FrameSourceBehavior::schemeList(void) { return QStringList(); }
bool FrameSourceBehavior::supportsScheme(const QString & scheme) { (void)scheme; return false; }
void FrameSourceBehavior::setPlugin(FrameSourcePlugin * plugin) { _plugin = plugin; }
bool FrameSourceBehavior::canGrab(void) { return false; }
void FrameSourceBehavior::preGrab(void) { FUNCTION(); }
VersionInfo FrameSourceBehavior::version(void) const { return _vi; }
void FrameSourceBehavior::setVersion(const VersionInfo & vi) { _vi = vi; }

void FrameSourceBehavior::enterConstruct(void) { FUNCTION(); FSPPOINTER(FrameSourcePlugin, _plugin); }
void FrameSourceBehavior::enterConnecting(void)  { FUNCTION(); }
void FrameSourceBehavior::enterConnected(void)  { FUNCTION(); }
void FrameSourceBehavior::enterConfiguring(void)  { FUNCTION(); }
void FrameSourceBehavior::enterConfigured(void)  { FUNCTION(); }
void FrameSourceBehavior::enterStarted(void)  { FUNCTION(); }
void FrameSourceBehavior::enterRunning(void)  { FUNCTION(); }
void FrameSourceBehavior::enterReady(void)  { FUNCTION(); }
void FrameSourceBehavior::enterGrabbing(void)  { FUNCTION(); }
void FrameSourceBehavior::enterRetry(void)  { FUNCTION(); }
void FrameSourceBehavior::enterPaused(void)  { FUNCTION(); }
void FrameSourceBehavior::enterFinished(void)  { FUNCTION(); }
void FrameSourceBehavior::enterError(void)  { FUNCTION(); }
void FrameSourceBehavior::enterFinal(void)  { FUNCTION(); }

void FrameSourceBehavior::exitConstruct(void)  { FUNCTION(); }
void FrameSourceBehavior::exitConnecting(void)  { FUNCTION(); }
void FrameSourceBehavior::exitConnected(void)  { FUNCTION(); }
void FrameSourceBehavior::exitConfiguring(void)  { FUNCTION(); }
void FrameSourceBehavior::exitConfigured(void)  { FUNCTION(); }
void FrameSourceBehavior::exitStarted(void)  { FUNCTION(); }
void FrameSourceBehavior::exitRunning(void)  { FUNCTION(); }
void FrameSourceBehavior::exitReady(void)  { FUNCTION(); }
void FrameSourceBehavior::exitGrabbing(void)  { FUNCTION(); }
void FrameSourceBehavior::exitRetry(void)  { FUNCTION(); }
void FrameSourceBehavior::exitPaused(void)  { FUNCTION(); }
void FrameSourceBehavior::exitFinished(void)  { FUNCTION(); }
void FrameSourceBehavior::exitError(void)  { FUNCTION(); }
void FrameSourceBehavior::exitFinal(void)  { FUNCTION(); }

void FrameSourceBehavior::connectResult(void)  { FUNCTION(); }
void FrameSourceBehavior::getResult(MillisecondTime mst,
                                    bool isError,
                                    ResultSet rset)  { FUNCTION(); }
