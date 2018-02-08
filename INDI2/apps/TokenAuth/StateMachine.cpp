#include "StateMachine.h"
#include "MainWindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFinalState>
#include <QtCore/QState>
#include <QtGui/QAction>

#include <eirCore/eirCore.h>
#include <eirExe/Log.h>
#include <eirGUI/CancelAction.h>
#include <eirGUI/Gallery.h>

#include "LivePage.h"
#include "MaintenancePage.h"

StateMachine::StateMachine(MainWindow * parent)
    : QStateMachine((QObject *)(parent))
    , initialize_state(new QState(this))
    , ready_state(new QState(this))
    , manualSearch_state(new QState(this))
    , searchResult_state(new QState(this))
    , personResult_state(new QState(this))
    , addEnroll_state(new QState(this))
    , enrollNew_state(new QState(this))
    , enrollPending_state(new QState(this))
    , authenticate_state(new QState(this))
    , authPass_state(new QState(this))
    , authFail_state(new QState(this))
    , authCancel_state(new QState(this))
    , authFinish_state(new QState(this))
    , confirm_state(new QState(this))
    , maintenance_state(new QState(this))
    , shutdown_state(new QState(this))
    , error_state(new QState(this))
    , final_state(new QFinalState(this))
{
    qDebug() << "TokenAuth-StateMachine ctor";
    setObjectName("TokenAuth-StateMachine");
}

MainWindow * StateMachine::mainw(void) const
{
    MainWindow * p = qobject_cast<MainWindow *>(parent());
    OBJPOINTER(MainWindow, p);
    return p;
}

LivePage * StateMachine::live(void) const
{
    return mainw()->live();
}

MaintenancePage * StateMachine::maint(void) const
{
    return mainw()->maint();
}

void StateMachine::initialize(void)
{
    qDebug() << "TokenAuth-StateMachine::initialize()";
    OBJPOINTER(QState, initialize_state);
    OBJPOINTER(QState, ready_state);
    OBJPOINTER(QState, manualSearch_state);
    OBJPOINTER(QState, searchResult_state);
    OBJPOINTER(QState, personResult_state);
    OBJPOINTER(QState, addEnroll_state);
    OBJPOINTER(QState, enrollNew_state);
    OBJPOINTER(QState, enrollPending_state);
    OBJPOINTER(QState, authenticate_state);
    OBJPOINTER(QState, authPass_state);
    OBJPOINTER(QState, authFail_state);
    OBJPOINTER(QState, authCancel_state);
    OBJPOINTER(QState, authFinish_state);
    OBJPOINTER(QState, confirm_state);
    OBJPOINTER(QState, maintenance_state);
    OBJPOINTER(QState, shutdown_state);
    OBJPOINTER(QState, error_state);
    OBJPOINTER(QFinalState, final_state);

    CONNECT(initialize_state, SIGNAL(entered()),
            mainw(), SLOT(enterInitialize()));
    CONNECT(initialize_state, SIGNAL(exited()),
            mainw(), SLOT(leaveInitialize()));
    CONNECT(ready_state, SIGNAL(entered()),
            live(), SLOT(enterReady()));
    CONNECT(ready_state, SIGNAL(exited()),
            live(), SLOT(leaveReady()));
    CONNECT(manualSearch_state, SIGNAL(entered()),
            live(), SLOT(enterManualSearch()));
    CONNECT(searchResult_state, SIGNAL(entered()),
            live(), SLOT(enterSearchResult()));
    CONNECT(searchResult_state, SIGNAL(exited()),
            live(), SLOT(leaveSearchResult()));
    CONNECT(personResult_state, SIGNAL(entered()),
            live(), SLOT(enterPersonResult()));
    CONNECT(personResult_state, SIGNAL(exited()),
            live(), SLOT(leavePersonResult()));
    CONNECT(addEnroll_state, SIGNAL(entered()),
            live(), SLOT(enterAddEnroll()));
    CONNECT(addEnroll_state, SIGNAL(exited()),
            live(), SLOT(leaveAddEnroll()));
    CONNECT(enrollNew_state, SIGNAL(entered()),
            live(), SLOT(enterEnrollNew()));
    CONNECT(enrollNew_state, SIGNAL(exited()),
            live(), SLOT(leaveEnrollNew()));
    CONNECT(enrollPending_state, SIGNAL(entered()),
            live(), SLOT(enterEnrollPending()));
    CONNECT(enrollPending_state, SIGNAL(exited()),
            live(), SLOT(leaveEnrollPending()));
    CONNECT(authenticate_state, SIGNAL(entered()),
            live(), SLOT(enterAuthenticate()));
    CONNECT(authPass_state, SIGNAL(entered()),
            live(), SLOT(enterAuthPass()));
    CONNECT(authFail_state, SIGNAL(entered()),
            live(), SLOT(enterAuthFail()));
    CONNECT(authCancel_state, SIGNAL(entered()),
            live(), SLOT(enterAuthCancel()));
    CONNECT(authFinish_state, SIGNAL(entered()),
            live(), SLOT(enterAuthFinish()));
    CONNECT(authFinish_state, SIGNAL(exited()),
            live(), SLOT(leaveAuthFinish()));
    CONNECT(confirm_state, SIGNAL(entered()),
            live(), SLOT(enterConfirm()));
    CONNECT(maintenance_state, SIGNAL(entered()),
            maint(), SLOT(enterMaintenance()));
    CONNECT(maintenance_state, SIGNAL(exited()),
            maint(), SLOT(leaveMaintenance()));
    CONNECT(shutdown_state, SIGNAL(entered()),
            mainw(), SLOT(enterShutdown()));
    CONNECT(error_state, SIGNAL(entered()),
            mainw(), SLOT(enterError()));

    initialize_state->addTransition(mainw(),
                                    SIGNAL(allStarted()),
                                    ready_state);
    initialize_state->addTransition(mainw(),
                                    SIGNAL(initializeError()),
                                    error_state);
    initialize_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    ready_state->addTransition(live(),
                               SIGNAL(doManualSearch()),
                               manualSearch_state);
    ready_state->addTransition(mainw(),
                               SIGNAL(gotToken()),
                               authenticate_state);
    ready_state->addTransition(qApp,
                               SIGNAL(aboutToQuit()),
                               shutdown_state);
    ready_state->addTransition(live()->maintenanceAction(),
                               SIGNAL(triggered()),
                               maintenance_state);
    manualSearch_state->addTransition(live(),
                                      SIGNAL(gotSearchResult()),
                                      searchResult_state);
    manualSearch_state->addTransition(live(),
                                      SIGNAL(searchResultError()),
                                      ready_state);
    manualSearch_state->addTransition(live(),
                                      SIGNAL(doEnrollNew()),
                                      enrollNew_state);
    manualSearch_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    searchResult_state->addTransition(live(),
                                      SIGNAL(endManualSearch()),
                                      ready_state);
    searchResult_state->addTransition(live()->bottomGallery(),
                                      SIGNAL(selected(GalleryItem*)),
                                      personResult_state);
    searchResult_state->addTransition(live(),
                                      SIGNAL(doEnrollNew()),
                                      enrollNew_state);
    searchResult_state->addTransition(qApp,
                                      SIGNAL(aboutToQuit()),
                                      shutdown_state);
    personResult_state->addTransition(live(),
                                      SIGNAL(endPersonResult()),
                                      searchResult_state);
    personResult_state->addTransition(live(),
                                      SIGNAL(doAddEnroll()),
                                      addEnroll_state);
    personResult_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    addEnroll_state->addTransition(live(),
                                   SIGNAL(endAddEnroll()),
                                   enrollPending_state);
    addEnroll_state->addTransition(live()->cancelAction(),
                                   SIGNAL(cancelled()),
                                   enrollPending_state);
    addEnroll_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    enrollNew_state->addTransition(live(),
                                   SIGNAL(endEnrollNew()),
                                   enrollPending_state);
    enrollNew_state->addTransition(live()->cancelAction(),
                                   SIGNAL(cancelled()),
                                   enrollPending_state);
    enrollNew_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    enrollPending_state->addTransition(live(),
                                       SIGNAL(gotEnrollResult()),
                                       ready_state);
    enrollPending_state->addTransition(live(),
                                       SIGNAL(enrollError()),
                                       ready_state);
    enrollPending_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    authenticate_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    authenticate_state->addTransition(live(),
                                      SIGNAL(gotAuthResult()),
                                      confirm_state);
    authenticate_state->addTransition(live(),
                                      SIGNAL(authResultError()),
                                      ready_state);
    authenticate_state->addTransition(live()->cancelAction(),
                                      SIGNAL(cancelled()),
                                      authCancel_state);
    authCancel_state->addTransition(ready_state);
    authFinish_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    confirm_state->addTransition(live(),
                                 SIGNAL(confirmed()),
                                 authPass_state);
    confirm_state->addTransition(live(),
                                 SIGNAL(rejected()),
                                 authFail_state);
    confirm_state->addTransition(live()->cancelAction(),
                                      SIGNAL(cancelled()),
                                      authCancel_state);
    authPass_state->addTransition(authFinish_state);
    authFail_state->addTransition(authFinish_state);
    authFinish_state->addTransition(live()->finishAction(),
                                    SIGNAL(toggled(bool)),
                                    ready_state);
    authFinish_state->addTransition(live()->cancelAction(),
                                      SIGNAL(cancelled()),
                                      authCancel_state);
    authFinish_state->addTransition(live(),
                               SIGNAL(doManualSearch()),
                               manualSearch_state);
    authFinish_state->addTransition(live(),
                                      SIGNAL(doEnrollNew()),
                                      enrollNew_state);
    authFinish_state->addTransition(live(),
                                      SIGNAL(doAddEnroll()),
                                      addEnroll_state);
    maintenance_state->addTransition(maint()->finishAction(),
                                     SIGNAL(triggered()),
                                     ready_state);
    maintenance_state->addTransition(qApp,
                                    SIGNAL(aboutToQuit()),
                                    shutdown_state);
    error_state->addTransition(shutdown_state);
    shutdown_state->addTransition(final_state);
    setInitialState(initialize_state);
}
