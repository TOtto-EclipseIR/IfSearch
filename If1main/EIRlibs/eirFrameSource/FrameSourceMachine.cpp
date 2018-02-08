#include "FrameSourceMachine.h"
#include "FrameSourcePlugin.h"

#include <QtCore/QFinalState>
#include <QtCore/QState>

#include "../eirExe/Log.h"
#include "../eirExe/Results.h"

#include "FrameSourceState.h"

FrameSourceMachine::FrameSourceMachine(FrameSourcePlugin * parent)
    : QStateMachine(parent)
    , _plugin(parent)
{
    FUNCTION();
}

ResultSet FrameSourceMachine::initialize(void)
{
    FUNCTION();

    /*===== Create States =====*/
    QState * constructState = new QState(this);
    QState * connectingState = new QState(this);
    QState * connectedState = new QState(this);
    QState * configuringState = new QState(this);
    QState * configuredState = new QState(this);
    QState * startedState = new QState(this);
    QState * errorState = new QState(this);
    QFinalState * finalState = new QFinalState(this);
    QStateMachine * runningMachine = new QStateMachine(this);
    OBJPOINTER(QState, constructState);
    OBJPOINTER(QState, connectingState);
    OBJPOINTER(QState, connectedState);
    OBJPOINTER(QState, configuringState);
    OBJPOINTER(QState, configuredState);
    OBJPOINTER(QState, startedState);
    OBJPOINTER(QState, errorState);
    OBJPOINTER(QFinalState, finalState);
    OBJPOINTER(QStateMachine, runningMachine);
    setInitialState(constructState);

    /*===== Setup Enter/Leave signals =====*/
    CONNECT(constructState, SIGNAL(entered()), _plugin, SLOT(enterConstruct()));
    CONNECT(connectingState, SIGNAL(entered()), _plugin, SLOT(enterConnecting()));
    CONNECT(connectedState, SIGNAL(entered()), _plugin, SLOT(enterConnected()));
    CONNECT(configuringState, SIGNAL(entered()), _plugin, SLOT(enterConfiguring()));
    CONNECT(configuredState, SIGNAL(entered()), _plugin, SLOT(enterConfigured()));
    CONNECT(startedState, SIGNAL(entered()), _plugin, SLOT(enterStarted()));
    CONNECT(errorState, SIGNAL(entered()), _plugin, SLOT(enterError()));
    CONNECT(finalState, SIGNAL(entered()), _plugin, SLOT(enterFinal()));
    CONNECT(runningMachine, SIGNAL(entered()), _plugin, SLOT(enterRunning()));
    CONNECT(constructState, SIGNAL(exited()), _plugin, SLOT(exitConstruct()));
    CONNECT(connectingState, SIGNAL(exited()), _plugin, SLOT(exitConnecting()));
    CONNECT(connectedState, SIGNAL(exited()), _plugin, SLOT(exitConnected()));
    CONNECT(configuringState, SIGNAL(exited()), _plugin, SLOT(exitConfiguring()));
    CONNECT(configuredState, SIGNAL(exited()), _plugin, SLOT(exitConfigured()));
    CONNECT(startedState, SIGNAL(exited()), _plugin, SLOT(exitStarted()));
    CONNECT(errorState, SIGNAL(exited()), _plugin, SLOT(exitError()));
    CONNECT(finalState, SIGNAL(exited()), _plugin, SLOT(exitFinal()));
    CONNECT(runningMachine, SIGNAL(exited()), _plugin, SLOT(exitRunning()));

    /*===== State Properties =====*/
    constructState->assignProperty(_plugin, "state", FrameSourceState::Construct);
    connectingState->assignProperty(_plugin, "state", FrameSourceState::Connecting);
    connectedState->assignProperty(_plugin, "state", FrameSourceState::Connected);
    configuringState->assignProperty(_plugin, "state", FrameSourceState::Configuring);
    configuredState->assignProperty(_plugin, "state", FrameSourceState::Configured);
    startedState->assignProperty(_plugin, "state", FrameSourceState::Starting);
    errorState->assignProperty(_plugin, "state", FrameSourceState::Error);
    runningMachine->assignProperty(_plugin, "state", FrameSourceState::Running);

    /*===== State Transitions =====*/
    constructState->addTransition(_plugin, SIGNAL(onConnect()), connectingState);
    connectingState->addTransition(_plugin, SIGNAL(connected()), connectedState);
    connectingState->addTransition(_plugin, SIGNAL(connectError()), errorState);
    connectedState->addTransition(_plugin, SIGNAL(onConfigure()), configuringState);
    configuredState->addTransition(_plugin, SIGNAL(onConfigure()), configuringState);
    configuringState->addTransition(configuredState);
    configuredState->addTransition(_plugin, SIGNAL(onStart()), startedState);
    startedState->addTransition(_plugin, SIGNAL(started()), runningMachine);
    startedState->addTransition(_plugin, SIGNAL(startError()), errorState);
    runningMachine->addTransition(_plugin, SIGNAL(complete()), configuredState);
    runningMachine->addTransition(_plugin, SIGNAL(onStop()), configuredState);
    runningMachine->addTransition(_plugin, SIGNAL(onShutdown()), finalState);
    runningMachine->addTransition(_plugin, SIGNAL(grabError()), errorState);
    errorState->addTransition(finalState);

    initRunning(runningMachine);
    setErrorState(errorState);

    return ResultSet();
}

ResultSet FrameSourceMachine::initRunning(QStateMachine * run)
{
    FUNCTION();

    /*===== Create States =====*/
    QState * readyState = new QState(run);
    QState * grabbingState = new QState(run);
    QState * retryState = new QState(run);
    QState * pausedState = new QState(run);
    QState * finishedState = new QState(run);
    QFinalState * doneState = new QFinalState(run);
    OBJPOINTER(QState, readyState);
    OBJPOINTER(QState, grabbingState);
    OBJPOINTER(QState, retryState);
    OBJPOINTER(QState, pausedState);
    OBJPOINTER(QState, finishedState);
    OBJPOINTER(QFinalState, doneState);
    run->setInitialState(readyState);

    /*===== Setup Enter/Leave signals =====*/
    CONNECT(readyState, SIGNAL(entered()), _plugin, SLOT(enterReady()));
    CONNECT(grabbingState, SIGNAL(entered()), _plugin, SLOT(enterGrabbing()));
    CONNECT(retryState, SIGNAL(entered()), _plugin, SLOT(enterRetry()));
    CONNECT(pausedState, SIGNAL(entered()), _plugin, SLOT(enterPaused()));
    CONNECT(finishedState, SIGNAL(entered()), _plugin, SLOT(enterFinished()));
    CONNECT(readyState, SIGNAL(exited()), _plugin, SLOT(exitReady()));
    CONNECT(grabbingState, SIGNAL(exited()), _plugin, SLOT(exitGrabbing()));
    CONNECT(retryState, SIGNAL(exited()), _plugin, SLOT(exitRetry()));
    CONNECT(pausedState, SIGNAL(exited()), _plugin, SLOT(exitPaused()));
    CONNECT(finishedState, SIGNAL(exited()), _plugin, SLOT(exitFinished()));

    /*===== State Properties =====*/
    readyState->assignProperty(_plugin, "state", FrameSourceState::Ready);
    grabbingState->assignProperty(_plugin, "state", FrameSourceState::Grabbing);
    retryState->assignProperty(_plugin, "state", FrameSourceState::Retry);
    pausedState->assignProperty(_plugin, "state", FrameSourceState::Paused);
    finishedState->assignProperty(_plugin, "state", FrameSourceState::Finished);

    /*===== State Transitions =====*/
    readyState->addTransition(_plugin, SIGNAL(doGrab()), grabbingState);
    grabbingState->addTransition(_plugin, SIGNAL(grabbed()), readyState);
    readyState->addTransition(_plugin, SIGNAL(onPause()), pausedState);
    readyState->addTransition(_plugin, SIGNAL(onAbort()), pausedState);
    pausedState->addTransition(_plugin, SIGNAL(onResume()), readyState);
    grabbingState->addTransition(_plugin, SIGNAL(onAbort()), pausedState);
    grabbingState->addTransition(_plugin, SIGNAL(complete()), finishedState);
    grabbingState->addTransition(_plugin, SIGNAL(grabTimeout()), retryState);
    retryState->addTransition(grabbingState);
    finishedState->addTransition(doneState);

    return ResultSet();
}
