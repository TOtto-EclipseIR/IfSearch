#ifndef KEYCLASS_H
#define KEYCLASS_H
#include "eirBase.h"

class KeyClassBehavior;

/*! @class KeyClass The KeyClass is used internally by the KeyTemplate mechanism.
  *
  * It identifies the key class and instanciates a behavior singleton as needed.
  * @see KeyTemplate for the details of the individual classes.
  */
class EIRBASESHARED_EXPORT KeyClass
{
public:
    enum KeyClasses
    {
        Null = 0,
        KeyClass16z,
        KeyClass16a,

        KeyClass32z,
        KeyClass32a,
        KeyClass32b,
        KeyClass32c,

        KeyClass64z,
        KeyClass64a,
        KeyClass64b,
        KeyClass64c,
        KeyClass64d,
        KeyClass64e,
        KeyClass64f,
        KeyClass64g,
    };

public:
    static KeyClassBehavior * instance(const KeyClasses keyClass);

private:
    static QVector<KeyClassBehavior *> classData_vec;
};

#endif // KEYCLASS_H
