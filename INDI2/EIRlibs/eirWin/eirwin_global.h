#ifndef EIRWIN_GLOBAL_H
#define EIRWIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRWIN_LIBRARY)
#  define EIRWINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRWINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRWIN_GLOBAL_H
