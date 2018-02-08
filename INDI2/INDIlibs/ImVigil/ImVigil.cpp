#include "ImVigil.h"

#include "../../EIRlibs/eirExe/Log.h"
#undef ERROR

#if defined(Q_WS_WIN)
# define _WIN32_IE 0x0500
# include <qt_windows.h>
# include <commctrl.h>
# include <objbase.h>
#endif

ImVigil::ImVigil()
{
    FUNCTION();
    EXPECTEQ(CoInitialize(NULL), S_OK);
}

ImVigil::~ImVigil()
{
    FUNCTION();
    CoUninitialize();
}
