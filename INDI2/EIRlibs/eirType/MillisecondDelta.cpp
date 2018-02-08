#include "MillisecondDelta.h"

#include <QDateTime>
#include <QTime>

const qint64 MillisecondDelta::minute_ms = 60 * 1000;
const qint64 MillisecondDelta::hour_ms = 60 * minute_ms;
const qint64 MillisecondDelta::day_ms = 24 * hour_ms;


MillisecondDelta::MillisecondDelta(void)
    : _ms(0)
{
}

MillisecondDelta::MillisecondDelta(qint64 ms)
    : _ms(ms)
{
}

bool MillisecondDelta::isNull(void) const
{
    return ! _ms;
}

MillisecondDelta::operator qint64(void) const
{
    return _ms;
}

qint64 MillisecondDelta::msec(void) const
{
    return _ms;
}

void MillisecondDelta::clear(void)
{
    _ms = 0;
}

qint64 MillisecondDelta::setMinutes(qreal mins)
{
    clear();
    return addMinutes(mins);
}

qint64 MillisecondDelta::addMinutes(qreal mins)
{
    return _ms += mins * qreal(minute_ms);
}

qint64 MillisecondDelta::setHours(qreal hrs)
{
    clear();
    return addHours(hrs);
}

qint64 MillisecondDelta::addHours(qreal hrs)
{
    return _ms += hrs * qreal(hour_ms);
}

QString MillisecondDelta::toString(const QString & format)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(_ms);
    QTime tm = dt.time();
    QString fmt = format;
    if (fmt.contains("d")) fmt.replace("d", QString::number(_ms / day_ms));
    return tm.toString(fmt);
}
