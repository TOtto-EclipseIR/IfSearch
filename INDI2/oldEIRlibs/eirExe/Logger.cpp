#include "Logger.h"

#include <QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "../eirBase/BaseLog.h"
#include "../eirCore/VariableId.h"
#include "../eirCore/VariableIdList.h"
#include "../eirCore/VariableSet.h"
#include "../eirCore/VariableSettingsList.h"
#include "Log.h"
#include "LogDevice.h"
#include "LogItem.h"
#include "LogOptions.h"
#include "Result.h"
#include "Results.h"

Logger * Logger::singleton = 0;
Logger::States Logger::state = Initial;

Logger * Logger::instance(void)
{
    if ( ! singleton)
    {
        if (qApp)
            singleton = new Logger(qApp), state = Instance;
    }
    return singleton;
}

void Logger::create(QObject * parent)
{
    if ( ! singleton)
        singleton = new Logger(parent), state = Instance;
}

Logger::Logger(QObject * parent)
    : QObject(parent)
    , app_thread(qApp ? qApp->thread() : 0)
{
    VariableSet defaultConfig("defaultConfig");
    defaultConfig.set("Device", "Troll");
    defaultConfig.set("Options", "LevelChar TimeOfDay");
    defaultConfig.set("Filter/0/Report", "Detail~");
    addDevice("default", defaultConfig);
}

Logger::~Logger()
{
    finish();
    singleton = 0;
    state = Initial;
}

void Logger::add(const Severity sev,
                 const QString & mod,
                 const QString & file,
                 const int ln,
                 const QString & fn,
                 const QString & fmt,
                 const QVariant & v1,
                 const QVariant & v2,
                 const QVariant & v3,
                 const QVariant & v4)
{
    QVariantList vl;
    if ( ! v4.isNull())
        vl << v1 << v2 << v3 << v4;
    else if ( ! v3.isNull())
        vl << v1 << v2 << v3;
    else if ( ! v2.isNull())
        vl << v1 << v2;
    else if ( ! v1.isNull())
        vl << v1;
    add(sev, mod, file, ln, fn, fmt, vl);
}

void Logger::add(const Severity sev,
                 const QString & mod,
                 const QString & file,
                 const int ln,
                 const QString & fn,
                 const QString & fmt,
                 const QVariantList & vl)
{
    LogItem li;
    li.setTime(MillisecondTime::current());
    li.setSeverity(sev);
    li.setModule(mod);
    li.setFileName(file);
    li.setFileLine(ln);
    li.setFunction(fn);
    li.set(qPrintable(fmt));
    li.set(vl);
    if (app_thread != QThread::currentThread())
        li.setThread(threadName(QThread::currentThread()));
    enqueue(li);
}

void Logger::add(LogItem & li)
{
    li.setTime(MillisecondTime::current());
    if (app_thread != QThread::currentThread())
        li.setThread(threadName(QThread::currentThread()));
    enqueue(li);
}

void Logger::add(const Result & r)
{
    LogItem li;
    li.setTime(r.time());
    li.setSeverity(r.severity());
    li.set(qPrintable("["+r.code().toString()+"]"+r.format()));
    li.set(r.vars());
    enqueue(li);
}

void Logger::add(const ResultSet & rl)
{
    DUMP("%1 Results at least %2 severity, max=%3",
         rl.size(), rl.minSeverity().name(),
         rl.maxSeverity().name());
    foreach (Result r, rl.list())
        add(r);
}

/** @fn dump()
  *
  * @todo Stringify QVariants
  */
void Logger::dump(const VariableSet & vset) const
{
    DUMP("VariableSet: name=%1, key=%2 0x%3, id=%4",
         vset.name(),
         vset.key(),
         QString::number(vset.key(), 16),
         vset.id());
    foreach (VariableId vid, vset.ids())
    {
        Variable vbl(vset.at(vid));
        DUMP("---%1 = {%2}", vbl.id(), vbl.var().toString());
    }
    for (int x = 0; x < vset.listSize(); ++x)
        DUMP("---%1. {%2}", x, vset.value(x).toString());
    if (vset.binarySize())
        DUMPHEX(vset.value().constData(), vset.binarySize());
}

void Logger::dump(const VariableSettingsList & vsl) const
{
    int x = 0;
    VariableSettings * vstgs = vsl.settings(x++);
    while (vstgs)
    {
        dump(*vstgs);
        vstgs = vsl.settings(x++);
    }
}

void Logger::dumpHex(const QString & mod,
                     const QString & file,
                     const int ln,
                     const QString & fn,
                     const QString & name,
                     void const * const ptr,
                     const unsigned size)
{
    QString hexPtr = QString::number((ulong)ptr, 16);
    QByteArray ba((const char *)ptr, size);
    add(Severity::DumpHex, mod, file, ln, fn,
        "HEXDUMP for %3 at %1 %2 bytes",
        hexPtr, size, name, ba);
}

bool Logger::expect(const QString & mod,
                    const QString & file,
                    const int ln,
                    const QString & fn,
                    const bool expnot,
                    const bool expr,
                    const QString & text,
                    const QString & text2)
{
    if (expnot == expr)
        add(Severity::Expect, mod, file, ln, fn,
            text2.isNull()
                ? "Expectation Failed: %1"
                : "Expected %1 " + QString(expnot ? "!=" : "==") + "%2",
            text, text2);
    return expr;
}

void Logger::configure(const VariableSet & config)
{
    foreach (QString name, deviceNames())
        removeDevice(name);
    foreach (VariableId section, config.sectionIds())
        addDevice(section, config.exportSection(section));
    if (state < Ready) state = Ready;
}

void Logger::removeDevice(const QString & name)
{
    name_device_map.remove(name);
}

void Logger::addDevice(const QString & name,
                       const VariableSet & config)
{
    LogDevice * ld = LogDevice::create(config, this);
    if (ld)
    {
        OBJPOINTER(LogDevice, ld);
        name_device_map.insert(name, ld);
        if (Running == state) ld->start();
    }
}

bool Logger::hasStdout(void) const
{
    return false;
}

bool Logger::hasStderr(void) const
{
    return false;
}

QList<LogDevice *> Logger::devices(void) const
{
    return name_device_map.values();
}

QStringList Logger::deviceNames(void) const
{
    return name_device_map.keys();
}

void Logger::start(void)
{
    foreach (LogDevice * ld, name_device_map.values())
        ld->start();
    state = Running;
    QTimer::singleShot(10, this, SLOT(pump()));
}

void Logger::finish(void)
{
    foreach (LogDevice * ld, name_device_map.values())
    {
        ld->flush();
        ld->finish();
    }
    state = Finished;
}

QString Logger::threadName(QThread * thread)
{
    if (thread_name_map.contains(thread))
        return thread_name_map.value(thread);

    QString newName = thread->objectName();
    if (newName.isEmpty())
        newName = thread->metaObject()->className();
    if (newName.isEmpty())
    {
        if (8 == sizeof(void *))
        {
            quint64 u = (quint64)thread;
            newName = QString::number(u, 16);
        }
        else
        {
            quint32 u = (quint64)thread;
            newName = QString::number(u, 16);
        }
    }

    QStringList existingNames = thread_name_map.values();
    if (existingNames.contains(newName))
    {
        int k = 2;
        while (existingNames.contains(newName + "+" + QString::number(k)))
            k++;
        newName = newName + "+" + QString::number(k);
    }

    thread_name_map.insert(thread, newName);
    return newName;
} // threadName()

void Logger::enqueue(const LogItem & li)
{
    QWriteLocker wl(&write_queue_lock);

    foreach (LogDevice * ld, name_device_map.values())
    {
        DeviceItem di(ld, li);
        write_queue.enqueue(di);
    }

    if (BaseLog::isFatal(li.getSeverity()))
        flush();

    BaseLog bl(li.getSeverity(), li.value(), li.values());

    QTimer::singleShot(10, this, SLOT(flush()));
}

void Logger::flush(void) // slot
{
    QWriteLocker wl(&write_queue_lock);
    while ( ! write_queue.isEmpty())
    {
        DeviceItem di = write_queue.dequeue();
        LogDevice * ld = di.first;
        LogItem li = di.second;
        ld->write(li);
    }

    foreach (LogDevice * ld, name_device_map.values())
        ld->flush();
}

void Logger::pump(void) // slot
{
    write_queue_lock.lockForRead();
    if (write_queue.isEmpty())
    {
        write_queue_lock.unlock();
        foreach (LogDevice * ld, name_device_map.values())
            ld->flush();
        if (Running == state)
            QTimer::singleShot(100, this, SLOT(pump()));
    }
    else
    {
        write_queue_lock.unlock();
        write_queue_lock.lockForWrite();
        DeviceItem di = write_queue.dequeue();
        write_queue_lock.unlock();
        LogDevice * ld = di.first;
        LogItem li = di.second;
        ld->write(li);
        QTimer::singleShot(10, this, SLOT(pump()));
    }
}
