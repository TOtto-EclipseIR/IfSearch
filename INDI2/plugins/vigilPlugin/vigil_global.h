#ifndef VIGILLIVE_GLOBAL_H
#define VIGILLIVE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIGIL_LIBRARY)
#  define VIGILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIGILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VIGILLIVE_GLOBAL_H
