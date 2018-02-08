#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
#include "eirWin.h"

class EIRWINSHARED_EXPORT SystemInfo
{
public:
    SystemInfo(void);
    void update(void);
    DWORD MemoryPageBytes(void) const;

private:
    SYSTEM_INFO _si;
};

#endif // SYSTEMINFO_H
