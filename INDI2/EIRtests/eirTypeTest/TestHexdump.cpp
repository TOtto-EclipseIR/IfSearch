#include <eirType/Hexdump.h>

void eirTypeTest::testHexdump(void)
{
    QString title("Hexdump'ping an int");
    QString hexLine("+00000000: 00000201                             ....");
    Hexdump hdInt(513, title);
    if (2 != hdInt.size()) { QFAIL("hdInt wront size()"); return; }
    foreach (QString s, hdInt)
        BDUMPVAR(s);
    QString one(hdInt.takeFirst());
    QString two(hdInt.takeFirst());
    QCOMPARE(one, title);
    QVERIFY(two.contains("0201"));
    QCOMPARE(two, hexLine);
}

