#include "KeySegment.h"

const UniversalKey KeySegment::maskA_key  =     0b1;
const UniversalKey KeySegment::maskB_key  =    0b11;
const UniversalKey KeySegment::maskC_key  =   0b111;
const UniversalKey KeySegment::maskD_key  =  0b1111;
const UniversalKey KeySegment::maskE_key  = 0b11111;
const UniversalKey KeySegment::valueA_key =     0b1;
const UniversalKey KeySegment::valueB_key =    0b10;
const UniversalKey KeySegment::valueC_key =   0b100;
const UniversalKey KeySegment::valueD_key =  0b1000;
const UniversalKey KeySegment::valueE_key = 0b10000;
const int KeySegment::bitsA_i = 4;
const int KeySegment::bitsB_i = 8;
const int KeySegment::bitsC_i = 12;
const int KeySegment::bitsD_i = 16;
const int KeySegment::bitsE_i = 20;


KeySegment::KeySegment(void)
    : _key(0)
{
}

KeySegment::KeySegment(const UniversalKey key)
    : _key(key)
{
}

bool KeySegment::isNull(void) const
{
    return UniversalKey(0) == _key;
}

int KeySegment::bits(void) const
{
    if ((maskE_key & _key) == valueE_key)   return bitsE_i;
    if ((maskD_key & _key) == valueD_key)   return bitsD_i;
    if ((maskC_key & _key) == valueC_key)   return bitsC_i;
    if ((maskB_key & _key) == valueB_key)   return bitsB_i;
    if ((maskA_key & _key) == valueA_key)   return bitsA_i;
    return 0;
}

int KeySegment::shift(void) const
{
    int i = bits();
    return i + (i >> 1);
}
