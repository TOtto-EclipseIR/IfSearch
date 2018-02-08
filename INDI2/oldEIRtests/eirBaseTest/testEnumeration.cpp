#include <eirBase/Enumeration.h>

#define TESTENUM_ENUMERATION(NV) \
            NV(First,   =1) \
            NV(Second,  =2) \
            NV(Another, =4) \
            NV(Last,    =16) \

class TestEnum : public Enumeration
{
    DECLARE_ENUMERATION(TestEnum, TESTENUM_ENUMERATION);
};

DEFINE_ENUMERATION(TestEnum, TESTENUM_ENUMERATION);

class AClass
{
public:
    AClass(const TestEnum te) : te_(te) {}
    TestEnum testEnum(void) const { return te_; }
    TestEnum te_;
};

void eirBaseTest::testEnumeration(void)
{
    TestEnum e;
    QCOMPARE(e.isNull(), true);
    QCOMPARE(e.isValid(), false);
    AClass ae(e);
    QCOMPARE(ae.testEnum().isNull(), true);

    TestEnum e1(1), ee(-1);
    QCOMPARE(e1.isValid(), true);
    QCOMPARE(ee.isValid(), false);
    TestEnum e2("second");
    QCOMPARE(e2.isValid(), true);
    QCOMPARE(e2.value(), 2);
    QCOMPARE(e2.name(), QString("Second"));
    TestEnum e3(TestEnum::Another);
    TestEnum e9(TestEnum::Last);

    switch (e1)
    {
    case TestEnum::First:
        QCOMPARE(e1.value(), 1);
        break;
    default:
        QCOMPARE(ee.isNull(), true);
    }

    e = e1;
    QCOMPARE(e.isValid(), true);
    QCOMPARE(e.value(), 1);
    QVERIFY(e == e1);
    QVERIFY(e1 == e.first());
    ++e;
    QCOMPARE(e.isValid(), true);
    QCOMPARE(e.value(), 2);
    QVERIFY(e != e1);
    QVERIFY(e == e2);
    QVERIFY(e != e.first());
    QVERIFY(++e == e3);
    QVERIFY(e++ == e3);
    QVERIFY(e == e9);
    QCOMPARE(e.isValid(), true);
    ++e;
    QCOMPARE(e.isValid(), false);

    TestEnum ef(e9.first());
    TestEnum el(ef.last());
    QCOMPARE(ef.isValid(), true);
    QCOMPARE(el.isValid(), true);
    QVERIFY(ef != el);
    QList<int> vl;
    e = ef;
    while (e.isValid())
    {
        vl << e++;
    }
    int s1 = vl.size();
    int s2 = ef.values().size();
    QCOMPARE(s1, s2);
    QCOMPARE(vl, ef.values());

    e = el;
    while (e.isValid())
    {
        QVERIFY(e <= e.last() && e >= e.first());
        --e;
    }

    QCOMPARE(TestEnum::base().parseFlags("~"), 0x17);
    QCOMPARE(TestEnum::base().parseFlags("first"), 1);
    QCOMPARE(TestEnum::base().parseFlags("another"), 4);
    QCOMPARE(TestEnum::base().parseFlags("last"), 0x10);
    QCOMPARE(TestEnum::base().parseFlags("~last"), 0x17);
    QCOMPARE(TestEnum::base().parseFlags("first~"), 0x17);
    QCOMPARE(TestEnum::base().parseFlags("first another second -another"), 3);
    QCOMPARE(TestEnum::base().parseFlags("first~second "), 3);
    QCOMPARE(TestEnum::base().parseFlags("first~another -second"), 5);
    QCOMPARE(TestEnum::base().parseFlags("first~second another"), 0x7);
    QCOMPARE(TestEnum::base().parseFlags("~ -another"), 0x13);
}

