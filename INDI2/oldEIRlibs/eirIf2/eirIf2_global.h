#ifndef EIRIF2_GLOBAL_H
#define EIRIF2_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRIF2_LIBRARY)
#  define EIRIF2SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRIF2SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRIF2_GLOBAL_H
