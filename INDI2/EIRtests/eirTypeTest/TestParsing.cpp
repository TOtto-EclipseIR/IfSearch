#include <eirType/Parsable.h>
#include <eirType/Parsed.h>

void eirTypeTest::testParsing(void)
{
    int i = -7325;
    Parsable pa(i);
    BTRACE(pa);
    Parsed pd(pa);
    QCOMPARE(pd.toInt(), i);

    unsigned u = 8123231;
    Parsable upa(u);
    BTRACE(upa);
    Parsed upd(upa);
    QCOMPARE(upd.toUInt(), u);
}

