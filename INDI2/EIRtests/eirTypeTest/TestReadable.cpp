#include <eirType/Readable.h>

void eirTypeTest::testReadable(void)
{
    Readable rInt(123);
    QCOMPARE(QString(rInt), QString("123"));
}
