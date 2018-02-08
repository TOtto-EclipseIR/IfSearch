#ifndef EIRKID_GLOBAL_H
#define EIRKID_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRKID_LIBRARY)
#  define EIRKIDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRKIDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRKID_GLOBAL_H
