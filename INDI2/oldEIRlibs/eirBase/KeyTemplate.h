#ifndef KEYTEMPLATE_H
#define KEYTEMPLATE_H

#include <QtDebug>

#include "KeyClass.h"
#include "KeyClassBehavior.h"

/*! @class KeyTemplate The KeyTemplate template provides a very flexible key mechanism.
  *
  * The keys are simple integers of 16, 32, or 64 bits.
  * They can be identified as to their type and class.
  *
  * @section Key Classes
  *
  * - 16 bit classes, 15 key types
  *     - KeyClass16z - 511 values
  *     - KeyClass16a - 7 groups of 63 values
  * - 32 bit classes, 47 key types
  *     - KeyClass32z - 2 million values
  *     - KeyClass32a - 15 groups of 128 thousdand values
  *     - KeyClass32b - 127 groups of 16,383 values
  *     - KeyClass32c - 1023 groups of 2,048 values
  * - 64 bit classes, 191 key types
  *     - KeyClass64z - 1 quadrillion values (2^50)
  *     - KeyClass64a - 255 groups of 4 trillion values
  *     - KeyClass64b - 2047 groups of 256 billion values
  *     - KeyClass64c - 32767 groups of 16 billion values
  *     - KeyClass64d - 1 million groups of 1 billion values
  *     - KeyClass64e - 16 million groups of 64 million values
  *     - KeyClass64f - 256 million groups of 4 million values
  *     - KeyClass64g - 4 billion groups of 256 thousand values
  *
  * @section Usage Usage
  *
  * @subsection KeyTypes KeyTypes
  * In eirBase/KeyTypes.h add an entry in the appropiate area (by key size).
  *
  * @subsection hFile Derived .h file
  * @code
class DerivedKey : public KeyTemplate<qint32, KeyTypes::Derived>
{
public:
    DerivedKey(void)
        : KeyTemplate<qint32, KeyTypes::Derived>() {}

    DerivedKey(const qint64 i)
        : KeyTemplate<qint32, KeyTypes::Derived>(i) {}

    DerivedKey(const qint64 l, const qint64 n)
        : KeyTemplate<qint32, KeyTypes::Derived>(l, n) {}
};
  * @endcode
  *
  * @subsection cppFile Derived .cpp file
  * @code
template<> KeyClassBehavior * KeyTemplate<qint32, KeyTypes::Derived>::behavior = KeyClass::instance(KeyClass::KeyClass32b);
  * @endcode
  */
template <typename INT, int KEYTYPE> class KeyTemplate
{
public:
    static bool isValid(const qint64 i)
    { return behavior->isValid(KEYTYPE, i); }
    KeyTemplate(void) : key_int(behavior->null()) {}
    KeyTemplate(const qint64 i)
        : key_int(INT(isValid(i) ? i : behavior->null())) {}
    KeyTemplate(const qint64 l, const qint64 n)
        : key_int(INT(behavior->construct(KEYTYPE, l, n))) {}
    void setKeyClass(const KeyClass::KeyClasses keyClass)
    { behavior = KeyClass::instance(keyClass); }
    INT key(void) const
    { return key_int; }
    bool isValid(void) const
    { return isValid(qint64(key_int)); }
    void dump(void) const
    { return behavior->dump(key_int); }
    int sizeValue(void) const
    { return key_int & 3; }
    int lValue(void) const
    { return behavior->lValue(qint64(key_int)); }
    int nValue(void) const
    { return behavior->nValue(qint64(key_int)); }
    int value(void) const
    { return behavior->value(qint64(key_int)); }

private:
    INT key_int;
    static KeyClassBehavior * behavior;
};



#endif // KEYTEMPLATE_H
