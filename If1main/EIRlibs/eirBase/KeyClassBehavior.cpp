#include "KeyClassBehavior.h"
#include "../eirBase/BaseLog.h"

KeyClassBehavior::KeyClassBehavior(const int intBits,
                                   const int sizeValue,
                                   const int keyTypeBits,
                                   const int classBits,
                                   const int classValue,
                                   const int nValueBits)
    : intBits_i(intBits)
    , sizeValue_i(sizeValue)
    , classValue_i(classValue)
{
    null_i64 = (1 << (intBits - 1));
    zeroMask_i64 = (0xFFFFFFFFFFFFFFFFLL << (intBits - 1));
    signatureMask_i64 = 0x3 | (((1 << classBits) - 1) << 2) | null_i64;
    signatureValue_i64 = sizeValue | (classValue << 2);
    keyTypeShift_i = 2 + classBits;
    keyTypeMask_i64 = ((1 << keyTypeBits) - 1) << keyTypeShift_i;
    nShift_i = keyTypeShift_i + keyTypeBits;
    lShift_i = nShift_i + nValueBits;
    nMask_i64 = nValueBits ? (((1 << nValueBits) - 1) << nShift_i) : 0;
    lMask_i64 = ((1 << (intBits - 3 - classBits - keyTypeBits - nValueBits)) - 1) << lShift_i;
}

bool KeyClassBehavior::isValid(const int keyType,
                               const qint64 i) const
{
    if (i & zeroMask_i64)
        return false;
    if (signatureValue_i64 != (i & signatureMask_i64))
        return false;
    if ((keyType << keyTypeShift_i) != (i & keyTypeMask_i64))
        return false;
    return true;
}

qint64 KeyClassBehavior::construct(const int keyType,
                                   const qint64 lPart,
                                   const qint64 nPart)
{
    qint64 tShifted = keyType << keyTypeShift_i;
    if (tShifted & ~ keyTypeMask_i64)
        return qint64(null_i64);

    qint64 lShifted = lPart << lShift_i;
    if (lShifted & ~ lMask_i64)
        return qint64(null_i64);

    if (nShift_i)
    {
        qint64 nShifted = nPart << nShift_i;
        if (nShifted & ~ nMask_i64)
            return qint64(null_i64);
        lShifted |= nShifted;
    }

    return sizeValue_i | (classValue_i << 2) | tShifted | lShifted;
}

void KeyClassBehavior::dump(const qint64 k) const
{
    (void)k;
}


qint64 KeyClassBehavior::null(void) const
{
    return null_i64;
}

qint64 KeyClassBehavior::keyType(const qint64 k) const
{
    return (k & keyTypeMask_i64) >> keyTypeShift_i;
}

qint64 KeyClassBehavior::lValue(const qint64 k) const
{
    return (k & lMask_i64) >> lShift_i;
}

qint64 KeyClassBehavior::nValue(const qint64 k) const
{
    return (k & lMask_i64) >> lShift_i;
}

qint64 KeyClassBehavior::value(const qint64 k) const
{
    return (k & (lMask_i64 | nMask_i64)) >> lShift_i;
}

