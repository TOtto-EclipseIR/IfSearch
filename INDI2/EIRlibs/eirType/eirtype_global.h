#ifndef EIRTYPE_GLOBAL_H
#define EIRTYPE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRTYPE_LIBRARY)
#  define EIRTYPESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRTYPESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRTYPE_GLOBAL_H
