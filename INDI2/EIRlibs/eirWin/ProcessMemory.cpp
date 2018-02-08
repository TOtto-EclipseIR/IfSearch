#include "ProcessMemory.h"

#include <QCoreApplication>

ProcessMemory::ProcessMemory(void)
{
    process_h = OpenProcess(PROCESS_QUERY_INFORMATION
                            | PROCESS_VM_READ, /* access */
                            false, /* inherit handle */
                            processId_i64);
    update();
}

bool ProcessMemory::update(void)
{
    _error.clear();
    bool success = GetProcessMemoryInfo(process_h, &_pmc, sizeof(_pmc));
    if ( ! success) _error.setFromSystemLastError();
    return success;
}

size_t ProcessMemory::peakBytes(void) const
{
    return _pmc.PeakWorkingSetSize;
}

size_t ProcessMemory::currentBytes(void) const
{
    return _pmc.WorkingSetSize;
}

size_t ProcessMemory::privateBytes(void) const
{
    return _pmc.PagefileUsage;
}


bool ProcessMemory::isError(void) const
{
    return _error.isError();
}

QString ProcessMemory::errorString(void) const
{
    return _error.string();
}
