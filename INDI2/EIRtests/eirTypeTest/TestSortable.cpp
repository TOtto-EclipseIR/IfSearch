#include <eirType/Sortable.h>

void eirTypeTest::testSortable(void)
{
    Sortable sInt(123);
    Sortable sNeg(-4567);
    Sortable sZed(0);
    QCOMPARE(QString(sInt), QString("+0000000123"));
    QCOMPARE(QString(sNeg), QString("-0000004567"));
    QCOMPARE(QString(sZed), QString("00000000000"));
}

