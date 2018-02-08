#ifndef EIRBASE_GLOBAL_H
#define EIRBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRBASE_LIBRARY)
#  define EIRBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRBASE_GLOBAL_H
