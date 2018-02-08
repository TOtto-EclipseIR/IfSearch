#include <QVariant>

#include <eirBase/Severity.h>
#include <eirBase/VersionInfo.h>

void eirBaseTest::testVariant(void)
{
    VersionInfo vi(1, 23, 45, 7, "v1.23+B45G",
                   "Copyright (c) 2014",
                   "Organization",
                   "Application");
    BDUMPVAR(qMetaTypeId<VersionInfo>());
    BUSERVAR(varVI, vi);
    BTRACE("varVI  %1 %2", varVI.type(), varVI.typeName());
    VersionInfo vi2 = varVI.value<VersionInfo>();
    BTRACE("vi  %1 %2", vi.toString(), vi.getBranch());
    BTRACE("vi2 %1 %2", vi2.toString(), vi2.getBranch());
    QCOMPARE(vi.toString(), vi2.toString());

    Severity sev(Severity::Stern);
    BDUMPVAR(qMetaTypeId<Severity>());
    BUSERVAR(var, sev);
    BTRACE("var  %1 %2", var.type(), var.typeName());
    Severity sev2 = var.value<Severity>();
    BTRACE("sev  %1 %2", sev.value(), sev.name());
    BTRACE("sev2 %1 %2", sev2.value(), sev2.name());
    QCOMPARE(sev, sev2);
}

