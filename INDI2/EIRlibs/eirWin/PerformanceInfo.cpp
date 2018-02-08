#include "PerformanceInfo.h"

#include "SystemInfo.h"

PerformanceInfo::PerformanceInfo(void)
    : systemInfo_p(new SystemInfo)
{
    update();
}

bool PerformanceInfo::update(void)
{
    _error.clear();
    bool success = GetPerformanceInfo(&_pi, sizeof(_pi));
    if ( ! success) _error.setFromSystemLastError();
    return success;
}

size_t PerformanceInfo::totalPhysicalBytes(void) const
{
    Q_ASSERT(systemInfo_p);
    return _pi.PhysicalTotal * size_t(systemInfo_p->MemoryPageBytes());
}

size_t PerformanceInfo::availablePhysicalBytes(void) const
{
    Q_ASSERT(systemInfo_p);
    return _pi.PhysicalAvailable * size_t(systemInfo_p->MemoryPageBytes());
}

qreal PerformanceInfo::availablePhysicalPercent(void) const
{
    size_t pa = availablePhysicalBytes();
    size_t pt = totalPhysicalBytes();
    return 100.0 * qreal(pa) / qreal(pt);
}

bool PerformanceInfo::isError(void) const
{
    return _error.isError();
}

QString PerformanceInfo::errorString(void) const
{
    return _error.string();
}
