#ifndef WINDOWSERROR_H
#define WINDOWSERROR_H
#include "eirWin.h"

#include <QString>

class WindowsError
{
public:
    WindowsError(void);
    void clear(void);
    bool isError(void) const;
    bool setFromSystemLastError(void);
    QString string(bool withHex=true) const;
    DWORD lastError(void) const;

private:
    DWORD error_dw;
    QString error_s;
};

#endif // WINDOWSERROR_H
