#ifdef EIRFRAMESOURCE_LIBRARY
#include "FrameSourcePlugin.h"
#endif
#ifdef VIGIL_LIBRARY
#include "vigilPlugin.h"
#endif
#ifdef BHTTPPLUGIN_LIBRARY
#include "bhttpPlugin.h"
#endif


#include <QtCore/QUrl>

#include "../eirBase/Severity.h"
#include "../eirExe/ResultId.h"
#include "../eirExe/Results.h"
#include "../eirExe/Log.h"

#include "FrameSourceBehavior.h"
#include "FrameSourceMachine.h"
#include "FrameSourceResult.h"
#include "FrameSourceState.h"
#include "FrameSourceStatusIndex.h"

bool FrameSourcePlugin::isNull(void) const
{
    return ( ! _behavior);
}

QStringList FrameSourcePlugin::schemeList(void)
{
    FUNCTION();
    NULPOINTER(_behavior);
    return _behavior->schemeList();
}

bool FrameSourcePlugin::supports(const QUrl & url)
{
    FUNCTION(url.toString());
    NULPOINTER(_behavior);
    bool b = _behavior->supports(url);
    FNRETURN(b);
    return b;
}

bool FrameSourcePlugin::supportsScheme(const QString & scheme)
{
    FUNCTION(scheme);
    NULPOINTER(_behavior);
    bool b = _behavior->supportsScheme(scheme);
    FNRETURN(b);
    return b;
}

VersionInfo FrameSourcePlugin::version(void) const
{
    FUNCTION();
    NULPOINTER(_behavior);
    FNRETURN(_behavior->version().toString());
    return _behavior->version();
}

void FrameSourcePlugin::setResultSeverity(const Severity sev)
{
    running_results.setMinSeverity(sev);
}

void FrameSourcePlugin::setBehavior(FrameSourceBehavior * beh)
{
    NULPOINTER(beh);
    _behavior = beh;
    _behavior->setPlugin(this);
    setObjectName(_behavior->name()+":FrameSourcePlugin");
}

void FrameSourcePlugin::startMachine(void)
{
    FUNCTION();
    NULPOINTER(_behavior);

    status_results.fill(FrameSourceStatusIndex::UserEnd);
    status_results.setMinSeverity(Severity::Warning);
    setStatus(FrameSourceStatusIndex::Initialization,
              Result(FrameSourceResult::Initializing,
                     _behavior->name()));

    if (_machine)
    {
        delete _machine;
        _machine = 0;
    }

    _machine = new FrameSourceMachine(this);
    OBJPOINTER(FrameSourceMachine, _machine);
    _machine->initialize();
    _machine->start();
}


VariableSet FrameSourcePlugin::configuration(void) const
{
    return config_vars;
}

Result FrameSourcePlugin::status(const int x) const
{
    return status_results.at(x);
}

ResultSet FrameSourcePlugin::status(void) const
{
    return status_results;
}

void FrameSourcePlugin::setStatus(const FrameSourceStatusIndex x,
                                  const Result & r)
{
    status_results.set(x, r);
    status_results.add(r);
}

ResultSet FrameSourcePlugin::results(void) const
{
    return running_results;
}

ResultSet FrameSourcePlugin::takeResults(void)
{
    FUNCTION();
    ResultSet results(running_results);
    running_results.clear();
    return results;
}

int FrameSourcePlugin::channel(void) const
{
    return channel_i;
}

MillisecondTime FrameSourcePlugin::beginMst(void) const
{
    return begin_mst;
}

MillisecondTime FrameSourcePlugin::endMst(void) const
{
    return end_mst;
}

bool FrameSourcePlugin::isInitialized(void) const
{
    return state() >= FrameSourceState::Initialized
            && state() < FrameSourceState::Error;
}

bool FrameSourcePlugin::isConfigured(void) const
{
    return state() >= FrameSourceState::Configured
            && state() < FrameSourceState::Error;
}

bool FrameSourcePlugin::isStarted(void) const
{
    return state() > FrameSourceState::Starting
            && state() < FrameSourceState::Error;
}

bool FrameSourcePlugin::isPaused(void) const
{
    return FrameSourceState::Paused == state();
}

bool FrameSourcePlugin::isFinished(void) const
{
    return FrameSourceState::Finished == state();
}

FrameSourceState FrameSourcePlugin::state(void) const
{
    return FrameSourceState(property("state").toInt());
}

QString FrameSourcePlugin::stateName(void) const
{
    return FrameSourceState(property("state").toInt()).name();
}

QUrl FrameSourcePlugin::url(void) const
{
    return _url;
}
void FrameSourcePlugin::emitInitialized(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(initialized(rset));
    EMIT(initialized());
}

void FrameSourcePlugin::emitConnected(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(connected(rset));
    EMIT(connected());
}

void FrameSourcePlugin::emitConnectError(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(connectError(rset));
    EMIT(connectError());
}

void FrameSourcePlugin::emitGrabbed(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(grabbed(rset));
    EMIT(grabbed());
}

void FrameSourcePlugin::emitGrabError(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(grabError(rset));
    EMIT(grabError());
}

void FrameSourcePlugin::emitGrabTimeout(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(grabTimeout(rset));
    EMIT(grabTimeout());
}

void FrameSourcePlugin::emitStarted(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(started(rset));
    EMIT(started());
}

void FrameSourcePlugin::emitStartError(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(startError(rset));
    EMIT(startError());
}

void FrameSourcePlugin::emitConfigured(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(configured(rset));
    EMIT(configured());
}

void FrameSourcePlugin::emitComplete(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    RESULTS(rset);
    EMIT(complete(rset));
    EMIT(complete());
}

void FrameSourcePlugin::enqueueGrab(const ImageEntity & ie)
{
    frame_image_list.append(ie);
}

QDomElement FrameSourcePlugin::enumerate(const int channel)
{
    FUNCTION();
    TODO("VariableSet::toQDomElement()");
    (void)channel;
    return QDomElement();
}

QString FrameSourcePlugin::channelId(const int channel)
{
    FUNCTION();
    TODO("Extract channelId from channel_vars");
    (void)channel;
    return QString();
}

void FrameSourcePlugin::connectResult(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->connectResult();
}

void FrameSourcePlugin::getResult(MillisecondTime mst,
                                   bool isError,
                                   ResultSet rset)
{
    FNSLOT(qint64(mst), isError, rset.size());
    NULPOINTER(_behavior);
    _behavior->getResult(mst, isError, rset);
}

void FrameSourcePlugin::httpProgress(MillisecondTime mst,
                  int count,
                  int total)
{
    FNSLOT(qint64(mst), count, total);

}
