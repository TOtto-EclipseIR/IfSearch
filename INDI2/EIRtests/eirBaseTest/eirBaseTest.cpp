#include <QString>
#include <QtTest>

#include <eirWin/PerformanceInfo.h>
#include <eirWin/ProcessMemory.h>
#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

class eirBaseTest : public QObject
{
    Q_OBJECT

public:
    eirBaseTest();

private Q_SLOTS:
    void testCase1();
    void testEnumeration(void);
    void testSeverity(void);
    void testSeverityMask(void);
    //void testSeverityArray(void);
    void testVariant(void);
    void cleanupTestCase(void);
};

eirBaseTest::eirBaseTest()
{
}

void eirBaseTest::cleanupTestCase()
{
    BTODO("Fix testSeverityArray()");
}

void eirBaseTest::testCase1()
{
    BLOG_MIN(Severity::Data);
    BaseLog::setFile("./eirBaseTest.blog");
    BLOGMSG(eirBase::instance()->name(),
            eirBase::instance()->version().toString());

    LMEMNEW(QObject, obj, this);
    obj->setObjectName("myObject");
    BDUMPVAR(obj->objectName());
    MEMBLOG(true);
    MEMFREE(obj);
    MEMFREE(obj);

    ProcessMemory processMemory;
    if (processMemory.isError())
    {
        BDUMPVAR(processMemory.errorString());
    }
    else
    {
        BDUMPVAR(processMemory.privateBytes());
        BDUMPVAR(processMemory.peakBytes());
        BDUMPVAR(processMemory.currentBytes());
    }

    PerformanceInfo performanceInfo;
    if (performanceInfo.isError())
    {
        BDUMPVAR(performanceInfo.errorString());
    }
    else
    {
        BDUMPVAR(performanceInfo.availablePhysicalBytes());
        BDUMPVAR(performanceInfo.totalPhysicalBytes());
        BDUMPVAR(performanceInfo.availablePhysicalPercent());
    }
}

#include "testEnumeration.cpp"
#include "testSeverity.cpp"
#include "testVariant.cpp"

QTEST_APPLESS_MAIN(eirBaseTest)

#include "eirBaseTest.moc"
