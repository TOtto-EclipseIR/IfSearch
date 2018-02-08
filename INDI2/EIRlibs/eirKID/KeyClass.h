#ifndef KEYCLASS_H
#define KEYCLASS_H
#include "eirKID.h"

#include <eirBase/Enumeration.h>

#define KEYCLASS_ENUM(NV) \
    NV(Null, = 0)  \
    NV(Object,)   \

class EIRKIDSHARED_EXPORT KeyClass : public Enumeration
{
    DECLARE_ENUMERATION(KeyClass, KEYCLASS_ENUM)
};

#endif // KEYCLASS_H
