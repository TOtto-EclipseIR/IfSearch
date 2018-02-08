#ifndef LOGITEM_H
#define LOGITEM_H
#include "eirLog.h"

#include <QSharedData>
#include <QVariantList>

#include <eirBase/DataProperty.h>
#include <eirBase/Severity.h>
#include <eirType/MillisecondTime.h>

#define LOGITEM_DATAPROPS(TND) \
    TND(MillisecondTime, Time,  MillisecondTime()) \
    TND(Severity, Severity,  Severity()) \
    TND(QString, ModuleName,  QString()) \
    TND(QString, FileName,  QString()) \
    TND(int, FileLine,  0) \
    TND(QString, ClassName,  QString()) \
    TND(QString, FunctionName,  QString()) \
    TND(QString, Format, QString()) \
    TND(QVariantList, Variables, QVariantList()) \
    TND(voidPtr, ThreadId,  0) \
    TND(int, Depth,  0) \

class LogItemData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(LOGITEM_DATAPROPS)
public:
    LogItemData(void)
    {
        DEFINE_DATAPROPS_CTORS(LOGITEM_DATAPROPS)
    }
};

class EIRLOGSHARED_EXPORT LogItem
{
    DECLARE_PARENT_DATAPROPS(LOGITEM_DATAPROPS)
    DECLARE_DATAPROPS(LogItem, LogItemData)
public:
    LogItem(const Severity severity,
            const QString & moduleName,
            const QString & fileName,
            const int fileLine,
            const QString & prettyFunction,
            const QString & format,
            const QVariant & v1=QVariant(),
            const QVariant & v2=QVariant(),
            const QVariant & v3=QVariant(),
            const QVariant & v4=QVariant(),
            const QVariant & v5=QVariant(),
            const QVariant & v6=QVariant(),
            const QVariant & v7=QVariant(),
            const QVariant & v8=QVariant(),
            const QVariant & v9=QVariant());
    void setPrettyFunction(const QString & prettyFunction);
    QString toString(const quint32 options);

private:
    QString severityString(const quint32 options) const;
    QString timeString(const quint32 options) const;
    QString locationString(const quint32 options) const;

private:
    QMap<quint32, QString> _opts_s_map;
};

#endif // LOGITEM_H
