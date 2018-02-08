#ifndef IMVIGIL_GLOBAL_H
#define IMVIGIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IMVIGIL_LIBRARY)
#  define IMVIGILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMVIGILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IMVIGIL_GLOBAL_H
