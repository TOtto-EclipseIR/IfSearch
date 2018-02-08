#include <eirBase/Severity.h>
#include <eirBase/SeverityArray.h>
#include <eirBase/SeverityMask.h>

#include <eirBase/BaseLog.h>

void eirBaseTest::testSeverity(void)
{
    Severity nul;
    QCOMPARE(nul.isNull(), true);
    QCOMPARE(nul.isValid(), false);

    Severity Minimum("Minimum");
    Severity Leave("Leave");
    Severity EnterArg("EnterArg");
    Severity Enter("Enter");
    Severity Signal("Signal");
    Severity Thread("Thread");
    Severity Alloc("Alloc");
    Severity Data("Data");
    Severity DumpHex("DumpHex");
    Severity DumpVar("DumpVar");
    Severity Dump("Dump");
    Severity Detail("Detail");
    Severity Trace("Trace");
    Severity Expect("Expect");
    Severity State("State");
    Severity Info("Info");
    Severity LogMsg("LogMsg");
    Severity Progress("Progress");
    Severity ToDo("ToDo");
    Severity Notice("Notice");
    Severity Watch("Watch");
    Severity Warning("Warning");
    Severity Stern("Stern");
    Severity Error("Error");
    Severity Pointer("Pointer");
    Severity Memory("Memory");
    Severity Assert("Assert");
    Severity Fatal("Fatal");
    Severity Shutdown("Shutdown");
    Severity Maximum("Maximum");

    QCOMPARE(Minimum.value(), 0);;
    QCOMPARE(Minimum.isNull(), false);
    QCOMPARE(Minimum.isValid(), true);
    QCOMPARE(Minimum.isFatal(), false);
    QCOMPARE(Minimum.isError(), false);
    QCOMPARE(Minimum.isWarning(), false);
    QCOMPARE(Leave.value(), 2);
    QCOMPARE(Leave.isNull(), false);
    QCOMPARE(Leave.isValid(), true);
    QCOMPARE(Leave.isFatal(), false);
    QCOMPARE(Leave.isError(), false);
    QCOMPARE(Leave.isWarning(), false);
    QCOMPARE(LogMsg.value(), 16);
    QCOMPARE(LogMsg.isNull(), false);
    QCOMPARE(LogMsg.isValid(), true);
    QCOMPARE(Detail.isFatal(), false);
    QCOMPARE(Detail.isError(), false);
    QCOMPARE(Detail.isWarning(), false);
    QCOMPARE(Trace.isFatal(), false);
    QCOMPARE(Trace.isError(), false);
    QCOMPARE(Trace.isWarning(), false);
    QCOMPARE(Progress.isFatal(), false);
    QCOMPARE(Progress.isError(), false);
    QCOMPARE(Progress.isWarning(), false);
    QCOMPARE(ToDo.isFatal(), false);
    QCOMPARE(ToDo.isError(), false);
    QCOMPARE(ToDo.isWarning(), true);
    QCOMPARE(Error.isFatal(), false);
    QCOMPARE(Error.isError(), true);
    QCOMPARE(Error.isWarning(), true);
    QCOMPARE(Pointer.isFatal(), true);
    QCOMPARE(Pointer.isError(), true);
    QCOMPARE(Pointer.isWarning(), true);
    QCOMPARE(Shutdown.value(), 29);
    QCOMPARE(Shutdown.isNull(), false);
    QCOMPARE(Shutdown.isValid(), true);
    QCOMPARE(Shutdown.isFatal(), true);
    QCOMPARE(Shutdown.isError(), true);
    QCOMPARE(Shutdown.isWarning(), true);

    BTRACE("cleared testSeverity()");
}

void eirBaseTest::testSeverityMask(void)
{
    SeverityMask nul;
    QCOMPARE(nul.isEmpty(), true);
    QCOMPARE(nul.is(Severity::Leave), false);
    QCOMPARE(nul.is(Severity::Shutdown), false);

    SeverityMask enter("~Signal");
    QCOMPARE(enter.isEmpty(), false);
    QCOMPARE(enter.is(Severity::Leave), true);
    QCOMPARE(enter.is(Severity::EnterArg), true);
    QCOMPARE(enter.is(Severity::Enter), true);
    QCOMPARE(enter.is(Severity::Signal), true);
    QCOMPARE(enter.is(Severity::Thread), false);
    QCOMPARE(enter.is(Severity::Trace), false);
    QCOMPARE(enter.is(Severity::Shutdown), false);

    SeverityMask user("Data Detail Info LogMsg Progress Notice "
                      "Warning Stern Error Fatal Kill Shutdown");
    QCOMPARE(user.isEmpty(), false);
    BLOG_MIN(Severity::Data);
    BDUMPVAR(user.mask());
    QCOMPARE(user.is(Severity::Leave), false);
    QCOMPARE(user.is(Severity::Alloc), false);
    QCOMPARE(user.is(Severity::Data), true);
    QCOMPARE(user.is(Severity::DumpHex), false);
    QCOMPARE(user.is(Severity::Detail), true);
    QCOMPARE(user.is(Severity::Trace), false);
    QCOMPARE(user.is(Severity::Info), true);
    QCOMPARE(user.is(Severity::LogMsg), true);
    QCOMPARE(user.is(Severity::Progress), true);
    QCOMPARE(user.is(Severity::ToDo), false);
    QCOMPARE(user.is(Severity::Notice), true);
    QCOMPARE(user.is(Severity::Watch), false);
    QCOMPARE(user.is(Severity::Warning), true);
    QCOMPARE(user.is(Severity::Stern), true);
    QCOMPARE(user.is(Severity::Error), true);
    QCOMPARE(user.is(Severity::Fatal), true);
    QCOMPARE(user.is(Severity::Kill), true);
    QCOMPARE(user.is(Severity::Memory), false);
    QCOMPARE(user.is(Severity::Shutdown), true);
}

#if 0
void eirBaseTest::testSeverityArray(void)
{
    SeverityArray<QString> sa0;
    QString s = sa0.at(Severity::Trace);
    bool b = sa0.isNull(Severity::Trace);
    QCOMPARE(s, QString());
    QCOMPARE(b, true);
    sa0.set("2", Severity::Trace, Severity::Stern);
    QCOMPARE(sa0.at(Severity::Minimum), QString());
    QCOMPARE(sa0.at(Severity::Detail), QString());
    QCOMPARE(sa0.at(Severity::Trace), QString("2"));
    QCOMPARE(sa0.at(Severity::Warning), QString("2"));
    QCOMPARE(sa0.at(Severity::Stern), QString("2"));
    QCOMPARE(sa0.at(Severity::Error), QString());
    QCOMPARE(sa0.at(Severity::Shutdown), QString());

    SeverityArray<QString> sa1("1");
    QCOMPARE(sa1.at(Severity::Trace), QString("1"));
    sa1.set(Severity::Trace, "3");
}
#endif
