#include "MillisecondTime.h"

#include "../eirBase/BaseLog.h"
#include "../eirCore/eirCore.h"
#include "MillisecondDelta.h"

const QDateTime MillisecondTime::base_dt(QDateTime::currentDateTime());
const MillisecondTime  MillisecondTime::base_mst(base_dt);
const MillisecondDelta MillisecondTime::tz_msd(base_dt.toUTC().toMSecsSinceEpoch() - base_mst);
const MillisecondTime MillisecondTime::null_mst(INT64_MIN);

MillisecondTime::MillisecondTime(void)
    : _ems(null_mst)
{
}

MillisecondTime::MillisecondTime(qint64 ems)
    : _ems(ems)
{
}

MillisecondTime::MillisecondTime(const QDateTime & dt)
    : _ems(dt.isValid() ? dt.toMSecsSinceEpoch() : INT64_MIN)
{
}

MillisecondTime MillisecondTime::current(void)
{
    return MillisecondTime(QDateTime::currentMSecsSinceEpoch());
}

MillisecondTime MillisecondTime::fromIso(const QString & isoDateTime)
{
    return MillisecondTime(QDateTime::fromString(isoDateTime, Qt::ISODate));
}

MillisecondTime MillisecondTime::fromString(const QString & dateTime)
{
    qint64 i64 = dateTime.toLongLong();
    return MillisecondTime((i64 > 3000)
                           ? QDateTime::fromMSecsSinceEpoch(i64)
                           : QDateTime::fromString(dateTime, Qt::ISODate));
}

bool MillisecondTime::isValid(void) const
{
    return toDateTime().isValid();
}

bool MillisecondTime::isNull(void) const
{
    return null_mst == _ems;
}

bool MillisecondTime::isBaseNull(void) const
{
    return null_mst == base_mst;
}

bool MillisecondTime::isUtc(void)
{
    return 0 == tz_msd;
}

MillisecondTime::operator qint64(void) const
{
    return _ems;
}

MillisecondTime & MillisecondTime::operator += (const MillisecondDelta d)
{
    BLOG(Severity::Dump, "before %lli", _ems);
    _ems += d;
    BLOG(Severity::Dump, "after %lli", _ems);
    return *this;
}

MillisecondTime & MillisecondTime::operator -= (const MillisecondDelta d)
{
    _ems -= d;
    return *this;
}

void MillisecondTime::set(const MillisecondTime other)
{
    _ems = other._ems;
}

void MillisecondTime::setNull(void)
{
    _ems = null_mst;
}

qint64 MillisecondTime::base(void)
{
    return base_mst;
}

MillisecondTime MillisecondTime::null(void)
{
    return null_mst;
}

MillisecondDelta MillisecondTime::delta(MillisecondTime ems) const
{
    return MillisecondDelta(ems - _ems);
}

MillisecondDelta MillisecondTime::deltaBase(MillisecondTime ems)
{
    return MillisecondDelta(ems - base_mst);
}

QDateTime MillisecondTime::toDateTime(void) const
{
    return QDateTime::fromMSecsSinceEpoch(_ems);
}

QString MillisecondTime::toString(const QString & format) const
{
    return toDateTime().toString(format.isEmpty()
                                 ? "yyyy/MM/dd hh:mm:ss.zzz"
                                 : format);
}

QString MillisecondTime::baseString(const QString & format)
{
    return QDateTime::fromMSecsSinceEpoch(base_mst).toString(format);
}
