#ifndef EIROBJDET_GLOBAL_H
#define EIROBJDET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIROBJDET_LIBRARY)
#  define EIROBJDETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIROBJDETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIROBJDET_GLOBAL_H
