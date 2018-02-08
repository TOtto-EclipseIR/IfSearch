#ifndef PERFORMANCEINFO_H
#define PERFORMANCEINFO_H
#include "eirWin.h"

#include "WindowsError.h"

class SystemInfo;

class EIRWINSHARED_EXPORT PerformanceInfo
{
public:
    PerformanceInfo(void);
    bool update(void);
    size_t totalPhysicalBytes(void) const;
    size_t availablePhysicalBytes(void) const;
    qreal availablePhysicalPercent(void) const;
    bool isError(void) const;
    QString errorString(void) const;

private:
    PERFORMANCE_INFORMATION _pi;
    SystemInfo * systemInfo_p;
    WindowsError _error;
};

#endif // PERFORMANCEINFO_H
