#include "MillisecondDelta.h"

#include <QtCore/QDateTime>
#include <QtCore/QTime>

const qint64 MillisecondDelta::day_ms = 24 * 60 * 60 * 1000;


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
    return !! _ms;
}

MillisecondDelta::operator qint64(void) const
{
    return _ms;
}

QString MillisecondDelta::toString(const QString & format)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(_ms);
    QTime tm = dt.time();
    QString fmt = format;
    if (fmt.contains("d")) fmt.replace("d", QString::number(_ms / day_ms));
    return tm.toString(fmt);
}
