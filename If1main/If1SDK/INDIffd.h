#ifndef INDIFFD_H
#define INDIFFD_H

#include <qglobal.h>

#ifdef INDIFFD_LIB
# define INDIFFD_EXPORT Q_DECL_EXPORT
#else
# define INDIFFD_EXPORT Q_DECL_IMPORT
#endif

class INDIFFD_EXPORT INDIffd
{
public:
    INDIffd();
    ~INDIffd();

private:

};

#endif // INDIFFD_H
