#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QObject>

#include <QVariant>
class QTimer;

#include <eirTypes/MillisecondTime.h>
#include <eirTypes/MillisecondDelta.h>

class WatchDog : public QObject
{
    Q_OBJECT
public:
    explicit WatchDog(QObject * parent=0);
    WatchDog(const MillisecondDelta interval,
             QObject * parent=0) { }
    bool isReadyToStart(void) const;
    bool isRunning(void) const;

signals:
    void started(void);
    void bite(QVariant lastValue,
              MillisecondTime lastTime);

public slots:
    void setInterval(MillisecondDelta interval);
    void start(void);
    void stop(void);
    void pet(QVariant value);

private slots:
    void timeout(void);

private:
    QTimer * timer_p;
    MillisecondDelta interval_msd;
    QVariant lastValue_var;
    MillisecondTime lastValue_mst;
    MillisecondTime bite_mst;
};

#endif // WATCHDOG_H
