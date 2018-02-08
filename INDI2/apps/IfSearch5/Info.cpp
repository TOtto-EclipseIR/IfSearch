/** @file Info.cpp
*	Definition of Info class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QCoreApplication>
#include <QTimer>

#include <Info.h>



#include <Return.h>

DECLARE_NAMEDENUM_INSTANCE(InfoSeverity)
DECLARE_NAMEDENUM_ID(InfoSeverity, Null)
DECLARE_NAMEDENUM_ID(InfoSeverity, Leave)
DECLARE_NAMEDENUM_ID(InfoSeverity, Enter)
DECLARE_NAMEDENUM_ID(InfoSeverity, Detail)
DECLARE_NAMEDENUM_ID(InfoSeverity, Debug)
DECLARE_NAMEDENUM_ID(InfoSeverity, Info)
DECLARE_NAMEDENUM_ID(InfoSeverity, Progress)
DECLARE_NAMEDENUM_ID(InfoSeverity, Warning)
DECLARE_NAMEDENUM_ID(InfoSeverity, Error)
DECLARE_NAMEDENUM_ID(InfoSeverity, Fatal)
DECLARE_NAMEDENUM_ID(InfoSeverity, Unknown)

DECLARE_NAMEDARRAY_INSTANCE(InfoSeverity, QList<InfoOutputBase *>)

QDateTime Info::TimeBase(QDateTime::currentDateTime());
QQueue<InfoItem> Info::queue;
QReadWriteLock Info::queueLock(QReadWriteLock::Recursive);
InfoOutputs Info::outputs;
QReadWriteLock Info::outputLock(QReadWriteLock::Recursive);
InfoThread * Info::thread = 0;
int Info::flushEvery_ = 10;
int Info::rolloverKeep_i = 0;

Info::~Info()
{
    flush();
    stop();
    // collect and delete the unique output handlers
    QSet<InfoOutputBase *> outs = outputs.unique();
    outputs.clear();
    foreach (InfoOutputBase * out, outs)
        delete out;
}

void Info::add(const Return & rtn)
{
    InfoSeverity sev = InfoSeverity::Info;
    switch (rtn.ddtSeverity())
    {
    case Null:			sev = InfoSeverity::Null;		break;
    case Leave:			sev = InfoSeverity::Leave;		break;
    case Enter:			sev = InfoSeverity::Enter;		break;
    case Detail:		sev = InfoSeverity::Detail;		break;
    case Debug:			sev = InfoSeverity::Debug;		break;
    case Progress:		sev = InfoSeverity::Progress;	break;
    case Warning:		sev = InfoSeverity::Warning;	break;
    case Error:			sev = InfoSeverity::Error;		break;
    case Fatal:			sev = InfoSeverity::Fatal;		break;
    case Unknown:		sev = InfoSeverity::Unknown;	break;
    }
    InfoItem ii(sev, QString(), 0, rtn.msg(), rtn[1], rtn[2], rtn[3], rtn[4]);
    ii.setReturnCode(rtn.returnCode());
    return add(ii);
}

bool Info::expect(const QString & strValue,
                  const QVariant & varValue,
                  const QString & strTest,
                  const QVariant & varTest,
                  const QString & file,
                  const int line)
{
    if (varValue == varTest)
        return true;

    add(InfoItem(InfoSeverity::Debug, file, line, "Expected %1 (%2)==(%3) %4",
                    strValue, varValue, varTest, strTest));
    return false;
}

bool Info::expectNot(const QString & strValue,
                     const QVariant & varValue,
                     const QString & strTest,
                     const QVariant & varTest,
                     const QString & file,
                     const int line)
{
    if (varValue != varTest)
        return true;

    add(InfoItem(InfoSeverity::Debug, file, line, "Expected %1 (%2)!=(%3) %4",
                    strValue, varValue, varTest, strTest));
    return false;
}

void Info::add(const InfoItem & item)
{
    {
        QWriteLocker lock(&queueLock);
        queue.enqueue(item);
    }
    if (flushEvery_ && size() >= flushEvery_)
        flush();
    if (item.severity() >= InfoSeverity::Fatal)
    {
        flush();
        qFatal("Shutdown by Fatal Error");
    }
}

void Info::add(InfoOutputBase * out,
               InfoSeverity minSev,
               InfoSeverity maxSev)
{
    remove(out);

    QWriteLocker lock(&outputLock);
    for (int x = minSev; x <= maxSev; ++x)
        outputs[x].append(out);
}

void Info::remove(InfoOutputBase * out)
{
    QWriteLocker lock(&outputLock);
    for (int x = InfoSeverity::min(); x <= InfoSeverity::max(); ++x)
        outputs[x].removeAll(out);
}

void Info::flush(void)
{
    while ( ! isEmpty())
        write(take());

    foreach (InfoOutputBase * out, outputs.unique())
        out->flush();
} // flush()

void Info::write(const InfoItem & item)
{
    foreach (InfoOutputBase * out, outputs.outputs(item.severity()))
        out->write(item);
}

void Info::clear(void)
{
    foreach (InfoOutputBase * out, outputs.unique())
        out->clear();
}

void Info::start(QThread::Priority priority, int msecShort, int msecLong)
{
    if (thread)
        stop();

    thread = new InfoThread(priority, msecShort, msecLong);
    if (thread)
#ifdef INFO_NOTHREAD
        QTimer::singleShot(msecShort, thread, SLOT(pump()));
#else
        thread->start();
#endif
} // start()

void Info::stop(void)
{
    if (thread)
    {
#ifndef INFO_THREAD
        thread->quit();
        thread->wait(5000);
        if (thread->isRunning())
        {
            thread->terminate();
            thread->wait(5000);
        }
#endif
        delete thread;
        thread = 0;
    }
} // stop()

void Info::rollover(void)
{
    flush();
    foreach (InfoOutputBase * out, outputs.unique())
    {
        out->rollover();
}
}
