#include <QString>
#include <QtTest>

#include <eirBase/BaseLog.h>
#include <eirKID/eirKID.h>

class TestKIDTest : public QObject
{
    Q_OBJECT

public:
    TestKIDTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();

    void TestBasicId(void);
};

TestKIDTest::TestKIDTest()
{
}

void TestKIDTest::initTestCase()
{
    BLOG_MIN(Severity::Data);
    //BaseLog::setFile("./TestKID.blog");
    BLOGMSG("%1 Module %2",
            eirKID::instance()->name(),
            eirKID::instance()->version().toString());
}

void TestKIDTest::cleanupTestCase()
{
}

void TestKIDTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

#include "TestBasicId.cpp"

QTEST_APPLESS_MAIN(TestKIDTest)

#include "tst_TestKIDTest.moc"
