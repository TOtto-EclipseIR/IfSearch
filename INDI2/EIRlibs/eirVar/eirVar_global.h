#ifndef EIRVAR_GLOBAL_H
#define EIRVAR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRVAR_LIBRARY)
#  define EIRVARSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRVARSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRVAR_GLOBAL_H
