#ifndef EIRIB2_GLOBAL_H
#define EIRIB2_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRIB2_LIBRARY)
#  define EIRIB2SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRIB2SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRIB2_GLOBAL_H
