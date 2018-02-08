#ifndef KEYCLASSBEHAVIOR_H
#define KEYCLASSBEHAVIOR_H
#include "eirBase.h"

/*! @class KeyClassBehavior The KeyClassBehavior is an internal support class for the KeyTemplate mechanism.
  */
class EIRBASESHARED_EXPORT KeyClassBehavior
{
public:
    KeyClassBehavior(const int intBits,
                     const int sizeValue,
                     const int keyTypeBits,
                     const int classBits,
                     const int classValue,
                     const int nValueBits);

    bool isValid(const int keyType,
                 const qint64 i) const;
    qint64 construct(const int keyType,
                     const qint64 lPart,
                     const qint64 nPart);
    qint64 null(void) const;
    void dump(const qint64 k) const;
    qint64 keyType(const qint64 k) const;
    qint64 lValue(const qint64 k) const;
    qint64 nValue(const qint64 k) const;
    qint64 value(const qint64 k) const;

private:
    int intBits_i;
    int sizeValue_i;
    int classValue_i;
    int keyTypeShift_i;
    int nShift_i;
    int lShift_i;
    qint64 null_i64;
    qint64 zeroMask_i64;
    qint64 signatureMask_i64;
    qint64 signatureValue_i64;
    qint64 keyTypeMask_i64;
    qint64 nMask_i64;
    qint64 lMask_i64;
};

#endif // KEYCLASSBEHAVIOR_H
