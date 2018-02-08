#include "TimeoutBar.h"

#include <QtCore/QDateTime>
#include <QtCore/QTimer>

#include "../eirTypes/MillisecondDelta.h"
#include "../eirExe/Log.h"

TimeoutBar::TimeoutBar(QWidget *parent)
    : QProgressBar(parent)
    , update_msec(200)
    , autoHide_b(true)
    , _timer(new QTimer(this))
{
    OBJPOINTER(QTimer, _timer);
    setObjectName("TimeoutBar");
    QProgressBar::setVisible(false);
    QProgressBar::setEnabled(false);
    QProgressBar::setTextVisible(false);
    CONNECT(_timer, SIGNAL(timeout()),
            this, SLOT(update()));
}

void TimeoutBar::start(int timeoutMsec)
{
    OBJPOINTER(QTimer, _timer);
    timeout_mst = MillisecondTime::current() + timeoutMsec;
    QProgressBar::setRange(-timeoutMsec, 0);
    QProgressBar::setValue(-timeoutMsec);
    QProgressBar::setEnabled(true);
    QProgressBar::setUpdatesEnabled(true);
    _timer->start(update_msec);
    QProgressBar::setVisible(true);
    emit started();
}

void TimeoutBar::stop(void)
{
    OBJPOINTER(QTimer, _timer);
    clear();
    emit stopped();
}

void TimeoutBar::clear(void)
{
    _timer->stop();
    QProgressBar::reset();
    QProgressBar::setEnabled(false);
    if (autoHide_b) QProgressBar::setVisible(false);
    emit cleared();
}

void TimeoutBar::update(void)
{
    if (timeout_mst.delta() > 0)
    {
        _timer->stop();
        QProgressBar::setValue(QProgressBar::maximum());
        emit timeout();
    }
    else
    {
        QProgressBar::setValue(timeout_mst.delta());
        emit updated();
    }
}
