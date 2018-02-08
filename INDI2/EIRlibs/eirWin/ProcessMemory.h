#ifndef PROCESSMEMORY_H
#define PROCESSMEMORY_H
#include "eirWin.h"

#include <QString>

#include "WindowsError.h"

class EIRWINSHARED_EXPORT ProcessMemory
{
public:
    ProcessMemory(void);
    bool update(void);
    size_t peakBytes(void) const;
    size_t currentBytes(void) const;
    size_t privateBytes(void) const;
    bool isError(void) const;
    QString errorString(void) const;

private:
    qint64 processId_i64;
    HANDLE process_h;
    PROCESS_MEMORY_COUNTERS _pmc;
    WindowsError _error;
};

#endif // PROCESSMEMORY_H
