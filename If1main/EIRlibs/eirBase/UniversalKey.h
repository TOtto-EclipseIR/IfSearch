#ifndef UNIVERSALKEY_H
#define UNIVERSALKEY_H
#include "eirBase.h"

#include "KeyClass.h"
class KeyClassBehavior;

class EIRBASESHARED_EXPORT UniversalKey
{
public:
    UniversalKey(const qint64 i);

private:
    quint64 _i64;
    KeyClass::KeyClasses _keyClass;
    KeyClassBehavior * behavior;

public:
    static const qint64 sizeMask_i64;
};

#endif // UNIVERSALKEY_H
