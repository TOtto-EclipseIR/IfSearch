#include "LogItem.h"

#include <QFileInfo>
#include <QThread>
#include <QVariantList>

#include <eirType/MillisecondDelta.h>
#include <eirType/Readable.h>

#include "LogFunction.h"
#include "LogOptions.h"

DEFINE_DATAPROPS(LogItem, LogItemData)

LogItem::LogItem(const Severity severity,
                 const QString & moduleName,
                 const QString & fileName,
                 const int fileLine,
                 const QString & prettyFunction,
                 const QString & format,
                 const QVariant & v1,
                 const QVariant & v2,
                 const QVariant & v3,
                 const QVariant & v4,
                 const QVariant & v5,
                 const QVariant & v6,
                 const QVariant & v7,
                 const QVariant & v8,
                 const QVariant & v9)
    : data(new LogItemData)
{
    ctor();
    setSeverity(severity);
    setModuleName(moduleName);
    setFileName(fileName);
    setFileLine(fileLine);
    setPrettyFunction(prettyFunction);
    setFormat(format);
    setVariables(QVariantList() << v1 << v2 << v3 << v4 << v5
                                << v6 << v7 << v8 << v9);
    setThreadId(QThread::currentThreadId());
}

void LogItem::ctor(void)
{
    setTime(MillisecondTime::current());
}

void LogItem::dtor(void) {}

void LogItem::setPrettyFunction(const QString & prettyFunction)
{
    LogFunction lfn(prettyFunction);
    setClassName(lfn.className());
    setFunctionName(lfn.functionName());
}

QString LogItem::toString(const quint32 options)
{
    if (_opts_s_map.contains(options))
        return _opts_s_map.value(options);

    QString severity(severityString(options));
    QString time(timeString(options));
    QString location(locationString(options));
    QString message(Readable::formatted(getFormat(),
                                        getVariables()));
    QString result(severity + time + location + message);
    _opts_s_map.insert(options, result);

    return result;
}

QString LogItem::severityString(const quint32 options) const
{
    if (options & LogOptions::NoLevel)
        return QString();
    if (options & LogOptions::LevelChar)
        return getSeverity().toChar();
    if (options & LogOptions::LevelName)
        return QString("%1 ").arg(getSeverity().name(), 8);
    return QString();
}

QString LogItem::timeString(const quint32 options) const
{
    if (options & LogOptions::NoTime)
        return QString();
    if (options & LogOptions::TimeOfDay)
        return getTime().toString("hh:mm:ss.zzz");
    if (options & LogOptions::TimeDelta)
        return MillisecondTime::deltaBase(getTime())
                .toString("hh:mm:ss.zzz");
    return QString();
}

QString LogItem::locationString(const quint32 options) const
{
    QString result(" ");
    if (options & LogOptions::ShowFile)
        result += QString("%1").arg(QFileInfo(getFileName())
                                    .completeBaseName(), 16);
    if (options & LogOptions::ShowLine)
        result += QString("[%1] ").arg(getFileLine(), -4);
    if (options & LogOptions::ShowClass)
        result += QString(" %1::").arg(getClassName(), 16);
    if (options & LogOptions::ShowFunction)
        result += QString("%1 ").arg(getFunctionName(), -16);
    if (options & LogOptions::ShowThread)
        result += QString("[0x%1] ")
                .arg((qptrdiff)getThreadId(), 16, 16, QChar('0'));
    return result;
}
