#ifndef SDKMACHINE_H
#define SDKMACHINE_H
#include "eirSDK1.h"

#include <QtCore/QStateMachine>

#include <QtCore/QString>
#include <QtCore/QStringList>
class QFinalState;
class QState;
class QTimer;

class SdkClient;
class SdkClientParameters;
class SdkCommand;
class TimeoutBar;
class VariableSettings;

class EIRSDK1SHARED_EXPORT SdkMachine : public QStateMachine
{
    Q_OBJECT

public:
    explicit SdkMachine(SdkClient * parent);
    ~SdkMachine();
    SdkClient * client(void) const;
    VariableSettings * interface(void) const;
    SdkClientParameters * parms(void) const;
    TimeoutBar * timeoutBar(QWidget * parent=0);
    void initialize(void);

public slots:
    void start(void);

signals:

private:
    void setupStates(void);
    void setupConnections(void);
    void setupTransitions(void);

private:
    QState * startProcess_state;
    QState * startSdk_state;
    QState * processError_state;
    QState * sdkTimeout_state;
    QState * returning_state;
    QState * paused_state;
    QState * running_state;
    QState * initiate_state;
    QState * pending_state;
    QState * complete_state;
    QState * commandTimeout_state;
    QFinalState * final_state;
    TimeoutBar * sdk_timer;
};

#endif // SDKMACHINE_H
