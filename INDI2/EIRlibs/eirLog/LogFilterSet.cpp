#include "LogFilterSet.h"

#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QUrlQuery>

#include <eirBase/BaseLog.h>

DEFINE_ENUMERATION(LogFilterKeyword, LOGFILTERKEYWORDS_ENUM);

LogFilterSet::LogFilterSet(void)
#ifdef QT_DECLARATIVE_DEBUG
    : fatal_sev(Severity::Stern)
#else
    : fatal_sev(Severity::Pointer)
#endif
{
}

void LogFilterSet::clear(void)
{
    _filter_list.clear();
}

bool LogFilterSet::isFatal(const Severity sev) const
{
    return sev.value() >= fatal_sev.value();
}

bool LogFilterSet::set(const QUrlQuery & query)
{
    typedef QPair<QString, QString> StringPair;
    QList<StringPair> items = query.queryItems();
    foreach (StringPair item, items)
    {
        QString key = item.first;
        QString value = item.second;
        QString name, severity;
        if (value.contains(':'))
        {
            QStringList sl = value.split(':');
            name = sl.at(0);
            severity = sl.at(1);
        }
        else
            severity = value;
        Severity sev(severity);
        SeverityMask sevMask;
        sevMask.set(severity);

        LogFilterKeyword keyword(key);
        if ( ! keyword.isValid())
        {
            BTRACE("%1 is not valid LogFilterKeyword", key);
            continue;
        }

        LogFilter lf;
        switch (keyword)
        {
        case LogFilterKeyword::Fatal:
            fatal_sev = sev;
            break;

        case LogFilterKeyword::Global:
            lf.setSeverityMask(sevMask);
            _filter_list.append(lf);
            break;

        case LogFilterKeyword::File:
            lf.setFileName(name);
            lf.setSeverityMask(sevMask);
            _filter_list.prepend(lf);
            break;

        case LogFilterKeyword::Module:
            lf.setModuleName(name);
            lf.setSeverityMask(sevMask);
            _filter_list.prepend(lf);
            break;

        case LogFilterKeyword::Function:
            lf.setFunctionName(name);
            lf.setSeverityMask(sevMask);
            _filter_list.prepend(lf);
            break;

        case LogFilterKeyword::Class:
            lf.setClassName(name);
            lf.setSeverityMask(sevMask);
            _filter_list.prepend(lf);
            break;

        case LogFilterKeyword::Options:
            break;
        }
    }
    return true;
} // set()

bool LogFilterSet::evaluate(const LogItem & li) const
{
    foreach (LogFilter lf, _filter_list)
        if (lf.evaluate(li))
            return true;
    return false;
}
