#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtDebug>

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>
#include <eirBase/VersionInfo.h>

class eirBaseTest : public QObject
{
    Q_OBJECT

public:
    eirBaseTest();

private Q_SLOTS:
    void testBase();
    void testEightCC(void);
    void testKeyTemplate(void);
    void testEnumeration(void);
    void testSeverity(void);
    void testSeverityMask(void);
};

eirBaseTest::eirBaseTest()
{
}

#include "testEightCC.cpp"
#include "testEnumeration.cpp"
#include "testSeverity.cpp"
#include "testKey.cpp"

void eirBaseTest::testBase()
{
    QVERIFY2(true, "Failure");
    QCOMPARE(eirBase::powerOfTwo(5), 8);
    qDebug() << eirBase::instance.version().getAppName()
                << eirBase::instance.version().toString();
    BaseLog::setFile("eirBaseTest.blog");
    qDebug() << "Started eirBaseTest.blog";
}

QTEST_APPLESS_MAIN(eirBaseTest);

#include "eirBaseTest.moc"
