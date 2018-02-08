#ifndef EIRCORE_GLOBAL_H
#define EIRCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRCORE_LIBRARY)
#  define EIRCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRCORE_GLOBAL_H
