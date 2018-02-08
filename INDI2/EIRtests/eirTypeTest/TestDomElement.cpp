#include <eirType/DomVariant.h>
#include <eirType/VariantDom.h>

void eirTypeTest::testDomElement(void)
{
    int i = -6464213;
    DomVariant dv(i);
    VariantDom vd(dv);
    QCOMPARE(vd.toInt(), i);
}


