#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QStateMachine>
class QFinalState;
class QState;

class MainWindow;
class LivePage;
class MaintenancePage;

class StateMachine : public QStateMachine
{
    Q_OBJECT
public:
    explicit StateMachine(MainWindow * parent);

public slots:
    void initialize(void);

private:
    MainWindow * mainw(void) const;
    LivePage * live(void) const;
    MaintenancePage * maint(void) const;

private:
    QState * initialize_state;
    QState * ready_state;
    QState * manualSearch_state;
    QState * searchResult_state;
    QState * personResult_state;
    QState * addEnroll_state;
    QState * enrollNew_state;
    QState * enrollPending_state;
    QState * authenticate_state;
    QState * authPass_state;
    QState * authFail_state;
    QState * authCancel_state;
    QState * authFinish_state;
    QState * confirm_state;
    QState * maintenance_state;
    QState * shutdown_state;
    QState * error_state;
    QFinalState * final_state;
};

#endif // STATEMACHINE_H
