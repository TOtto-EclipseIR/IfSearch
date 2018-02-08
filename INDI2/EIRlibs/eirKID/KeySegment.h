#ifndef KEYSEGMENT_H
#define KEYSEGMENT_H

#include "UniversalKey.h"

class KeySegment
{
public:
    KeySegment(void);
    KeySegment(const UniversalKey key);
    bool isNull(void) const;
    int bits(void) const;
    int shift(void) const;

private:
    UniversalKey _key;

private:
    static const UniversalKey maskA_key;
    static const UniversalKey maskB_key;
    static const UniversalKey maskC_key;
    static const UniversalKey maskD_key;
    static const UniversalKey maskE_key;
    static const UniversalKey valueA_key;
    static const UniversalKey valueB_key;
    static const UniversalKey valueC_key;
    static const UniversalKey valueD_key;
    static const UniversalKey valueE_key;
    static const int bitsA_i;
    static const int bitsB_i;
    static const int bitsC_i;
    static const int bitsD_i;
    static const int bitsE_i;
};

#endif // KEYSEGMENT_H
