#ifndef LOGFILTER_H
#define LOGFILTER_H
#include "eirLog.h"

#include <QSharedData>
#include <QString>
#include <QUrlQuery>

#include <eirBase/DataProperty.h>
#include <eirBase/Severity.h>
#include <eirBase/SeverityMask.h>


#define LOGFILTER_DATAPROPS(TND) \
    TND(QString,        ModuleName,     QString()) \
    TND(QString,        ClassName,      QString()) \
    TND(QString,        FunctionName,   QString()) \
    TND(QString,        FileName,       QString()) \
    TND(SeverityMask,   SeverityMask,   SeverityMask()) \

class LogFilterData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(LOGFILTER_DATAPROPS)
public:
    LogFilterData(void)
    {
        DEFINE_DATAPROPS_CTORS(LOGFILTER_DATAPROPS)
    }
};

class LogItem;

class EIRLOGSHARED_EXPORT LogFilter
{
    DECLARE_PARENT_DATAPROPS(LOGFILTER_DATAPROPS)
public:
    LogFilter(void); // default c'tor
    LogFilter(const LogFilter & rhs);
    LogFilter & operator = (const LogFilter & rhs);
    ~LogFilter();
    bool set(const QUrlQuery & query);
    bool evaluate(const LogItem & li) const;

private:
    QSharedDataPointer<LogFilterData> data;
};

#endif // LOGFILTER_H
