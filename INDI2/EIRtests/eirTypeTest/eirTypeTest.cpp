#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>
#include <eirType/FixedPoint.h>

class eirTypeTest : public QObject
{
    Q_OBJECT

public:
    eirTypeTest();

private Q_SLOTS:
    void initTestCase();
    void testCase1();
    void testReadable(void);
    void testSortable(void);
    void testHexdump(void);
    void testParsing(void);
    void testDomElement(void);
    void testCircularQueue(void);
    void cleanupTestCase();
};

eirTypeTest::eirTypeTest()
{
}

void eirTypeTest::initTestCase()
{
    typedef FixedPoint<qreal, qint16, 2046> sixteen;
    sixteen s0;
}

void eirTypeTest::cleanupTestCase()
{
}

void eirTypeTest::testCase1()
{
    QVERIFY2(true, "Failure");
    BLOG_MIN(Severity::Leave);
    BaseLog::setFile("./eirTypeTest.blog");
    BLOGMSG("%1 library %2",
            eirType::instance()->name(),
            eirType::instance()->version().toString(true));
}

QTEST_MAIN(eirTypeTest)

#include "TestReadable.cpp"
#include "TestSortable.cpp"
#include "TestHexdump.cpp"
#include "TestParsing.cpp"
#include "TestDomElement.cpp"
#include "TestCircularQueue.cpp"
#include "eirTypeTest.moc"

