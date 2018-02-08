#ifndef EIRFILE_GLOBAL_H
#define EIRFILE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRFILE_LIBRARY)
#  define EIRFILESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRFILESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRFILE_GLOBAL_H
