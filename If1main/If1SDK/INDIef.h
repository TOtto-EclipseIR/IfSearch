#ifndef INDIEF_H
#define INDIEF_H

#include <qglobal.h>

#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif


class INDIEF_EXPORT INDIef
{
public:
    INDIef();
    ~INDIef();

private:

};

#endif // INDIEF_H
