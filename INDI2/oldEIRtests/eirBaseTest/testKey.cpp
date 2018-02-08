#include <eirBase/BaseLog.h>
#include <eirBase/KeyTypes.h>
#include <eirBase/KeyTemplate.h>

class TestKey : public KeyTemplate<qint32, KeyTypes::Test>
{
public:
    TestKey(void)
        : KeyTemplate<qint32, KeyTypes::Test>() {}

    TestKey(const qint64 i)
        : KeyTemplate<qint32, KeyTypes::Test>(i) {}

    TestKey(const qint64 l, const qint64 n)
        : KeyTemplate<qint32, KeyTypes::Test>(l, n) {}
};

template<> KeyClassBehavior * KeyTemplate<qint32, KeyTypes::Test>::behavior
    = KeyClass::instance(KeyClass::KeyClass32b);

void eirBaseTest::testKeyTemplate()
{
    TestKey nullTestKey;
    QCOMPARE(TestKey::isValid(nullTestKey.key()), false);

    TestKey TestKey11(1, 1);
    TestKey11.dump();
    QCOMPARE(TestKey11.isValid(), true);
    QCOMPARE(TestKey11.sizeValue(), 2);
    QCOMPARE(TestKey11.lValue(), 1);
    QCOMPARE(TestKey11.nValue(), 1);
    QCOMPARE(TestKey11.key(), 0x0004091a);

    TestKey keyCopy(TestKey11.key());
    QCOMPARE(keyCopy.isValid(), true);
    QCOMPARE(keyCopy.sizeValue(), 2);
    QCOMPARE(keyCopy.lValue(), 1);
    QCOMPARE(keyCopy.nValue(), 1);
    QCOMPARE(keyCopy.key(), 0x0004091a);

    TestKey keyDirect(0x0004091a);
    QCOMPARE(keyDirect.isValid(), true);
    QCOMPARE(keyDirect.sizeValue(), 2);
    QCOMPARE(keyDirect.lValue(), 1);
    QCOMPARE(keyDirect.nValue(), 1);
    QCOMPARE(keyDirect.key(), 0x0004091a);
}
