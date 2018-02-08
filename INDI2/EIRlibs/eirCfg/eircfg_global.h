#ifndef EIRCFG_GLOBAL_H
#define EIRCFG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EIRCFG_LIBRARY)
#  define EIRCFGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRCFGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EIRCFG_GLOBAL_H
