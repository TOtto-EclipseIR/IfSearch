#ifndef UNIVERSALKEY_H
#define UNIVERSALKEY_H
#include "eirKID.h"

#include "KeyClass.h"

typedef quint64 UniversalKeyType;

class EIRKIDSHARED_EXPORT UniversalKey
{
public:
    UniversalKey(void);
    UniversalKey(const UniversalKeyType key);
    UniversalKey(const KeyClass keyClass);
    operator UniversalKeyType (void);
    bool isNull(void) const;
    bool operator == (const UniversalKey other) const;
    UniversalKey operator &  (const UniversalKey other) const;

private:
    UniversalKeyType _key;
};

#endif // UNIVERSALKEY_H
