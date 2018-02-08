#include <eirBase/EightCC.h>

void eirBaseTest::testEightCC(void)
{
    EightCC zz(0x3736353433323130LL);
    EightCC z7("01234567");
    QCOMPARE(zz, z7);
    QVERIFY(zz == z7);
    EightCC o;
    QVERIFY(o.isNull());
    QVERIFY( ! o.isPrintable());
    o = zz;
    QVERIFY( ! o.isNull());
    QCOMPARE(o, z7);
    QVERIFY(o.isPrintable());
    qDebug("%s %s %s",
           qPrintable(zz.toString()), z7(),
           qPrintable(o.toString()));
    EightCC zx(-1);
    QVERIFY( ! zx.isPrintable());
}

