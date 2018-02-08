#include <QtGlobal>
#include <QtCore/QSet>

template <class TRAITS> class Key
{
public:
    Key(void) : Value(0) {}
    Key(Key_type v) : Value(isValid(v) ? v : 0) {}
    bool isNull(void) const { return 0 == Value; }
    bool isValid(void) const
    { return Value >= TRAITS::MinValue && Value <= TRAITS::MaxValue; }
    operator Key_type (void) const
    { return Value; }

public:
    static bool isValid(Key_type v)
    { return v >= TRAITS::MinValue && v <= TRAITS::MaxValue; }
    static void take(Key_type v)
    { if (isValid(v)) TRAITS::Set.insert(v); }
    static Key newKey(void)
    {
        Key_type result;
        Key_type range = TRAITS::MaxValue - TRAITS::MinValue;
        if (range < 16) return Key(0);
        do
        {
            result = 0;
            range = TRAITS::MaxValue - TRAITS::MinValue;
            while (range)
            {
                result = (result << 4) | (0xF & qrand());
                range >>= 4;
            }
            result += TRAITS::MinValue;
        } while ( ! isValid(result) || TRAITS::Set.contains(result));
        return Key(result);
    }

private:
    Key_type Value;
};

#define KEYTRAITS_DECLARE(Name)	\
class Name	\
{	\
public:	\
    static void setRange(Key_type min, Key_type max)	\
    { MinValue = min, MaxValue = max; }	\
    static Key_type MinValue;	\
    static Key_type MaxValue;	\
    static QSet<Key_type> Set;  \
};

#define KEYTRAITS_INSTANCE(Name)	\
Key_type Name::MinValue = 0;		\
Key_type Name::MaxValue = 0;        \
QSet<Key_type> Name::Set;
