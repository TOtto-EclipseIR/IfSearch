#include "SdkMachine.h"

#include <QtCore/QFinalState>
#include <QtCore/QProcess>
#include <QtCore/QState>
#include <QtCore/QTimer>

#include "../eirExe/Log.h"
#include "../eirGUI/TimeoutBar.h"
#include "SdkClient.h"
#include "SdkCommand.h"
#include "SdkClientParameters.h"

SdkMachine::SdkMachine(SdkClient * parent)
    : QStateMachine(parent)
    , startProcess_state(new QState(this))
    , startSdk_state(new QState(this))
    , processError_state(new QState(this))
    , sdkTimeout_state(new QState(this))
    , returning_state(new QState(this))
    , paused_state(new QState(this))
    , running_state(new QState(this))
    , initiate_state(new QState(this))
    , pending_state(new QState(this))
    , complete_state(new QState(this))
    , commandTimeout_state(new QState(this))
    , final_state(new QFinalState(this))
    , sdk_timer(new TimeoutBar)
{
    qDebug() << "SdkMachine c'tor";
    setObjectName("SdkMachine");
    setProperty("State", "ctor");
}

SdkMachine::~SdkMachine()
{
    if (sdk_timer)  delete sdk_timer;
}

SdkClient * SdkMachine::client(void) const
{
    SdkClient * p = qobject_cast<SdkClient *>(parent());
    OBJPOINTER(SdkClient, p);
    return p;
}

VariableSettings * SdkMachine::interface(void) const
{
    return client()->interface();
}

SdkClientParameters * SdkMachine::parms(void) const
{
    return client()->parms();
}

TimeoutBar * SdkMachine::timeoutBar(QWidget * parent)
{
    OBJPOINTER(TimeoutBar, sdk_timer);
    if (parent) sdk_timer->setParent(parent);
    return sdk_timer;
}

void SdkMachine::initialize(void)
{
    qDebug() << objectName() << "SdkMachine::initialize()";
    OBJPOINTER(QState, startProcess_state);
    OBJPOINTER(QState, startSdk_state);
    OBJPOINTER(QState, processError_state);
    OBJPOINTER(QState, sdkTimeout_state);
    OBJPOINTER(QState, returning_state);
    OBJPOINTER(QState, paused_state);
    OBJPOINTER(QState, running_state);
    OBJPOINTER(QState, initiate_state);
    OBJPOINTER(QState, pending_state);
    OBJPOINTER(QState, complete_state);
    OBJPOINTER(QState, commandTimeout_state);
    OBJPOINTER(QFinalState, final_state);

    setupStates();
    setupConnections();
    setupTransitions();
    setInitialState(startProcess_state);
}

void SdkMachine::start(void)
{
    OBJPOINTER(TimeoutBar, sdk_timer);
    qDebug() << objectName() << "SdkMachine::start() timeout msec:"
             << parms()->getInitialize_TimeoutMsec();
    sdk_timer->start(parms()->getInitialize_TimeoutMsec());
    QStateMachine::start();
}

void SdkMachine::setupStates(void)
{
    startProcess_state->setObjectName("startProcess_state");
    startSdk_state->setObjectName("startSdk_state");
    processError_state->setObjectName("processError_state");
    sdkTimeout_state->setObjectName("sdkTimeout_state");
    returning_state->setObjectName("returning_state");
    paused_state->setObjectName("paused_state");
    running_state->setObjectName("running_state");
    initiate_state->setObjectName("initiate_state");
    pending_state->setObjectName("pending_state");
    complete_state->setObjectName("complete_state");
    commandTimeout_state->setObjectName("commandTimeout_state");
    final_state->setObjectName("final_state");
    startProcess_state->assignProperty(this, "State", "Start Process");
    startSdk_state->assignProperty(this, "State", "Start SDK");
    processError_state->assignProperty(this, "State", "Process Error");
    sdkTimeout_state->assignProperty(this, "State", "SDK Timeout");
    returning_state->assignProperty(this, "State", "Returning");
    paused_state->assignProperty(this, "State", "Paused");
    running_state->assignProperty(this, "State", "Running");
    initiate_state->assignProperty(this, "State", "Command Initiate");
    pending_state->assignProperty(this, "State", "Command Pending");
    complete_state->assignProperty(this, "State", "Command Complete");
    commandTimeout_state->assignProperty(this, "State", "Command Timeout");
}

void SdkMachine::setupConnections(void)
{
    CONNECT(startProcess_state, SIGNAL(entered()),
            client(), SLOT(enterStartProcess()));
    CONNECT(startSdk_state, SIGNAL(entered()),
            client(), SLOT(enterStartSdk()));
    CONNECT(processError_state, SIGNAL(entered()),
            client(), SLOT(enterProcessError()));
    CONNECT(sdkTimeout_state, SIGNAL(entered()),
            client(), SLOT(enterSdkTimeout()));
    CONNECT(returning_state, SIGNAL(entered()),
            client(), SLOT(enterReturning()));
    CONNECT(paused_state, SIGNAL(entered()),
            client(), SLOT(enterPaused()));
    CONNECT(running_state, SIGNAL(entered()),
            client(), SLOT(enterRunning()));
    CONNECT(initiate_state, SIGNAL(entered()),
            client(), SLOT(enterInitiate()));
    CONNECT(pending_state, SIGNAL(entered()),
            client(), SLOT(enterPending()));
    CONNECT(complete_state, SIGNAL(entered()),
            client(), SLOT(enterComplete()));
    CONNECT(commandTimeout_state, SIGNAL(entered()),
            client(), SLOT(enterCommandTimeout()));
}

void SdkMachine::setupTransitions(void)
{
    setInitialState(startProcess_state);
    startProcess_state->addTransition(client()->process(), SIGNAL(started()), startSdk_state);
    startProcess_state->addTransition(client()->process(), SIGNAL(error(QProcess::ProcessError)), processError_state);
    startProcess_state->addTransition(client(), SIGNAL(error(QString)), final_state);
    startProcess_state->addTransition(sdk_timer, SIGNAL(timeout()), sdkTimeout_state);
    startSdk_state->addTransition(client()->process(), SIGNAL(error(QProcess::ProcessError)), processError_state);
    startSdk_state->addTransition(sdk_timer, SIGNAL(timeout()), sdkTimeout_state);
    startSdk_state->addTransition(client(), SIGNAL(initialized()), returning_state);
    sdkTimeout_state->addTransition(client(), SIGNAL(error(QString)), final_state);
    processError_state->addTransition(client(), SIGNAL(error(QString)), final_state);

    returning_state->addTransition(client(), SIGNAL(commandQueued()), initiate_state);
    returning_state->addTransition(client(), SIGNAL(doPause()), paused_state);
    returning_state->addTransition(client(), SIGNAL(doResume()), running_state);
    running_state->addTransition(client(), SIGNAL(doPause()), paused_state);
    paused_state->addTransition(client(), SIGNAL(doResume()), running_state);
    running_state->addTransition(client(), SIGNAL(commandQueued()), initiate_state);
    paused_state->addTransition(client(), SIGNAL(commandQueued()), initiate_state);

    initiate_state->addTransition(client(), SIGNAL(empty()), returning_state);
    initiate_state->addTransition(pending_state);
    pending_state->addTransition(client(), SIGNAL(notifyComplete()), complete_state);
    pending_state->addTransition(sdk_timer, SIGNAL(timeout()), commandTimeout_state);
    complete_state->addTransition(returning_state);
    commandTimeout_state->addTransition(returning_state);
}
