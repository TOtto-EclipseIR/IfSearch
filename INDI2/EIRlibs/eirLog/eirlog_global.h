#ifndef EIRLOG_GLOBAL_H
#define EIRLOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRLOG_LIBRARY)
#  define EIRLOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRLOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRLOG_GLOBAL_H
