#include "UniversalKey.h"

#include <eirType/MillisecondTime.h>

UniversalKey::UniversalKey(void)
    : _key(0)
{
}

UniversalKey::UniversalKey(const UniversalKeyType key)
    : _key(key)
{
}

UniversalKey::UniversalKey(const KeyClass keyClass)
    : _key(keyClass.value()
           | (MillisecondTime::current() << 4))
{
}

UniversalKey::operator UniversalKeyType (void)
{
    return _key;
}

bool UniversalKey::isNull(void) const
{
    return !! _key;
}

bool UniversalKey::operator == (const UniversalKey other) const
{
    return _key == other._key;
}

UniversalKey UniversalKey::operator &(const UniversalKey other) const
{
    return _key & other._key;
}
