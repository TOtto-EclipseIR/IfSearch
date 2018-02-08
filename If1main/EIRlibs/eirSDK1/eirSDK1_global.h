#ifndef EIRSDK1_GLOBAL_H
#define EIRSDK1_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRSDK1_LIBRARY)
#  define EIRSDK1SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRSDK1SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRSDK1_GLOBAL_H
