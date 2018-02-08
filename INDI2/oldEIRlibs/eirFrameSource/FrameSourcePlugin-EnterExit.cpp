#include "FrameSourcePlugin.h"

#include <QtCore/QUrl>

#include "../eirBase/Severity.h"
#include "../eirExe/ResultId.h"
#include "../eirExe/Results.h"
#include "../eirExe/Log.h"

#include "FrameSourceBehavior.h"
#include "FrameSourceMachine.h"
#include "FrameSourceState.h"
#include "FrameSourceStatusIndex.h"


void FrameSourcePlugin::enterConstruct(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterConstruct();
}

void FrameSourcePlugin::enterConnecting(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterConnecting();
    if (status(FrameSourceStatusIndex::FirstConnection).isNull())
        setStatus(FrameSourceStatusIndex::FirstConnection,
                  Result(ResultId("eir", "FrameSource", "Connecting"),
                         _url));
    if (status(FrameSourceStatusIndex::LastConnection).isNull())
        setStatus(FrameSourceStatusIndex::LastConnection,
                  Result(ResultId("eir", "FrameSource", "Connecting"),
                         _url));
}

void FrameSourcePlugin::enterConnected(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterConnected();
}

void FrameSourcePlugin::enterConfiguring(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterConfiguring();
}
void FrameSourcePlugin::enterConfigured(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterConfigured();
}

void FrameSourcePlugin::enterStarted(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterStarted();
}

void FrameSourcePlugin::enterRunning(void)
{
    FNSLOT();

    if ( ! config_vars.value("Paused").toBool())
    {
        NULPOINTER(_behavior);
        _behavior->enterRunning();
        return;
    }

    SHOOT(50, this, SLOT(onPause()));
    //QTimer::singleShot(50, this, SLOT(onPause()));
}

void FrameSourcePlugin::enterReady(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterReady();
}

void FrameSourcePlugin::enterGrabbing(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterGrabbing();
}

void FrameSourcePlugin::enterRetry(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterRetry();
}

void FrameSourcePlugin::enterPaused(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterPaused();
}

void FrameSourcePlugin::enterFinished(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterFinished();
}

void FrameSourcePlugin::enterError(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterError();
}

void FrameSourcePlugin::enterFinal(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->enterFinal();
}


void FrameSourcePlugin::exitConstruct(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitConstruct();
}

void FrameSourcePlugin::exitConnecting(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitConnecting();
}

void FrameSourcePlugin::exitConnected(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitConnected();
}

void FrameSourcePlugin::exitConfiguring(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitConfiguring();
}

void FrameSourcePlugin::exitConfigured(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitConfigured();
}

void FrameSourcePlugin::exitStarted(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitStarted();
}

void FrameSourcePlugin::exitRunning(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitRunning();
}

void FrameSourcePlugin::exitReady(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitReady();
}

void FrameSourcePlugin::exitGrabbing(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitGrabbing();
}

void FrameSourcePlugin::exitRetry(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitRetry();
}

void FrameSourcePlugin::exitPaused(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitPaused();
}

void FrameSourcePlugin::exitFinished(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitFinished();
}

void FrameSourcePlugin::exitError(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitError();
}

void FrameSourcePlugin::exitFinal(void)
{
    FNSLOT();
    NULPOINTER(_behavior);
    _behavior->exitFinal();
}
