#pragma once

#include <QtCore/qglobal.h>

#if defined(EIRCORE_LIBRARY)
#  define EIRCORE_EXPORT Q_DECL_EXPORT
#else
#  define EIRCORE_EXPORT Q_DECL_IMPORT
#endif
