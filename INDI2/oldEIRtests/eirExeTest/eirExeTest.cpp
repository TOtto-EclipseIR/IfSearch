#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#define __MODULE__ "eirExeTest"
#include <eirCore/VariableSet.h>
#include <eirExe/Log.h>
#include <eirExe/LogFilter.h>
#include <eirExe/LogItem.h>

class eirExeTest : public QObject
{
    Q_OBJECT

public:
    eirExeTest(void);

private Q_SLOTS:
    void testExe(void);
    void testLogger(void);
};

void eirExeTest::testLogger(void)
{
    FUNCTION();
    DATA("DATA %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    DETAIL("DETAIL %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    TRACE("TRACE %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    NOTRACE("NOTRACE %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    STATE("STATE %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    INFO("INFO %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    LOGMSG("LOGMSG %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    PROGRESS("PROGRESS %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    TODO("TODO %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    NOTICE("NOTICE %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    WARNING("WARNING %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    STERN("STERN %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    ERROR("ERROR %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    //FATAL("FATAL %1 %2 %3 %4", 12, "ThirtyFour", 4.56, -789);
    if ( ! EXPECT(false)) QVERIFY2(true, "EXPECT ok");
    int one = 1;
    int another = 1;
    if ( ! EXPECTEQ(one, another)) QVERIFY2(true, "EXPECTEQ ok");
    WATCH(true);
    ASSERT(true);
    WATCH(false);

    VariableSet vset("Filter");
    vset.set("Report", "~info");
    DUMPVSET(vset);
    LogFilter lf(vset);
    DUMPVSET(lf);
    LogItem li("Item");
    li.setSeverity(Severity::Progress);
    DUMPVSET(li);
    QVERIFY(lf.evaluate(li));
    li.setSeverity(Severity::Enter);
    DUMPVSET(li);
    QVERIFY( ! lf.evaluate(li));
}

eirExeTest::eirExeTest()
{
    VariableSet logConfig("logConfig");
    logConfig.set("LogFile/Device", "TxtFile");
    logConfig.set("LogFile/FileName", "./log/eirExeTest-@.log");
    logConfig.set("LogFile/Options", "LevelChar TimeOfDay ShowFile");
    logConfig.set("LogFile/Filter/0/Report", "~");
    Logger::create(this);
    LOG(configure(logConfig));
    LOG(start());
}

void eirExeTest::testExe()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(eirExeTest)

#include "eirExeTest.moc"
