#include "WindowsError.h"

WindowsError::WindowsError(void)
    : error_dw(0)
{
}

void WindowsError::clear(void)
{
    error_dw = 0;
    error_s.clear();
}

bool WindowsError::isError(void) const
{
    return error_dw;
}

bool WindowsError::setFromSystemLastError(void)
{
    error_dw = GetLastError();
    wchar_t buffer[512];
    DWORD rtn = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                                0, /* no source buffer */
                                error_dw,
                                LOCALE_SYSTEM_DEFAULT, /* language */
                                buffer,
                                sizeof(buffer) / sizeof(TCHAR),
                                0 /* arguments */);
    if (rtn)
    {
        error_s = "{Unknown Windows System Error}";
        return false;
    }
    else
    {
        error_s = QString::fromUtf16((const ushort *)buffer);
        return true;
    }
}

QString WindowsError::string(bool withHex /*=true*/) const
{
    QString result(error_s);
    if (withHex)
        result.append(QString(" [0x%1]").arg(error_dw, 8, 16, QChar('0')));
    return result;
}

DWORD WindowsError::lastError(void) const
{
    return error_dw;
}
