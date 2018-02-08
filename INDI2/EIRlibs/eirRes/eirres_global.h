#ifndef EIRRES_GLOBAL_H
#define EIRRES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRRES_LIBRARY)
#  define EIRRESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRRESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRRES_GLOBAL_H
