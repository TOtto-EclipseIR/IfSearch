#include "WatchDog.h"

#include <QTimer>

WatchDog::WatchDog(QObject *parent)
    : QObject(parent)
    , timer_p(0)
{
    setObjectName("WatchDog");
}

bool WatchDog::isReadyToStart(void) const
{
    return ( !! interval_msd && ! timer_p);
}

bool WatchDog::isRunning(void) const
{
    return timer_p ? timer_p->isActive() : false;
}

void WatchDog::setInterval(MillisecondDelta interval)
{
    bool wasRunning = isRunning();
    stop();
    interval_msd = interval;
    if (wasRunning) start();
}

void WatchDog::start(void)
{
    stop();
    bite_mst = MillisecondTime::null();

    if (interval_msd)
    {
        timer_p = new QTimer(this);
        timer_p->setObjectName("WatchDog:QTimer");
        timer_p->setInterval(interval_msd);
        timer_p->setSingleShot(true);
        connect(timer_p, SIGNAL(timeout()), this, SLOT(timeout()));
        QObject::dumpObjectInfo();
        timer_p->start();
        emit started();
    }
}

void WatchDog::stop(void)
{
    if (timer_p)
    {
        timer_p->stop();
        delete timer_p;
        timer_p = 0;
    }
}

void WatchDog::pet(QVariant value)
{
    lastValue_var = value;
    lastValue_mst = MillisecondTime::current();
    if (timer_p) timer_p->start();
}

void WatchDog::timeout(void)
{
    stop();
    bite_mst = MillisecondTime::current();
    emit bite(lastValue_var, lastValue_mst);
}
