#ifndef FRAMESOURCEMACHINE_H
#define FRAMESOURCEMACHINE_H
#include "eirFrameSource.h"

#include <QStateMachine>

class ResultSet;

class FrameSourcePlugin;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceMachine : public QStateMachine
{
    Q_OBJECT
public:
    explicit FrameSourceMachine(FrameSourcePlugin * parent);
    ResultSet initialize(void);

private:
    ResultSet initRunning(QStateMachine *run);
    
signals:
    
public slots:
    
private:
    FrameSourcePlugin * _plugin;
};

#endif // FRAMESOURCEMACHINE_H
