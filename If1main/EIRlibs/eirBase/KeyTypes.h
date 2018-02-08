#ifndef KEYTYPES_H
#define KEYTYPES_H
#include "eirBase.h"

#include "Enumeration.h"

#define KEYTYPES_ENUM(NV) \
    NV(Class16base, = 0)  \
    NV(Class32base, = 16)  \
    NV(Test, ) \
    NV(Class64base, = 64)  \
    NV(Maximum,)   \

/*! @class KeyTypes The KeyTypes enumeration is used to provide unique key type values for the KeyTemplate mechanism.
  *
  * Add an item to the #define in the appropriate section by the size of the intended key.
  */
class EIRBASESHARED_EXPORT KeyTypes : public Enumeration
{
    DECLARE_ENUMERATION(KeyTypes, KEYTYPES_ENUM)
};

#endif // KEYTYPES_H
