#include "SystemInfo.h"

SystemInfo::SystemInfo(void)
{
    update();
}

void SystemInfo::update(void)
{
    GetSystemInfo(&_si);
}

DWORD SystemInfo::MemoryPageBytes(void) const
{
    return _si.dwPageSize;
}
