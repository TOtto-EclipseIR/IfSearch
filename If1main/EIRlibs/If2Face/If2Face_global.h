#ifndef IF2FACE_GLOBAL_H
#define IF2FACE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IF2FACE_LIBRARY)
#  define IF2FACESHARED_EXPORT Q_DECL_EXPORT
#else
#  define IF2FACESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IF2FACE_GLOBAL_H
