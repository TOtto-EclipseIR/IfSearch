#ifndef EIRWORK_GLOBAL_H
#define EIRWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRWORK_LIBRARY)
#  define EIRWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRWORK_GLOBAL_H
