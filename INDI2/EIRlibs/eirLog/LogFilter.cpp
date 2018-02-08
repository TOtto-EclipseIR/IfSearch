#include "LogFilter.h"

#include <QSharedData>

#include "LogItem.h"

LogFilter::LogFilter() : data(new LogFilterData)
{
}

LogFilter::LogFilter(const LogFilter &rhs) : data(rhs.data)
{
}

LogFilter & LogFilter::operator = (const LogFilter &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

LogFilter::~LogFilter()
{
}

bool LogFilter::evaluate(const LogItem & li) const
{
    if ( ! getSeverityMask().is(li.getSeverity()))
        return false;
    if ( ! getModuleName().isEmpty()
         && getModuleName().compare(li.getModuleName(),
                                    Qt::CaseInsensitive))
        return false;
    if ( ! getClassName().isEmpty()
         && getClassName().compare(li.getClassName(),
                                    Qt::CaseInsensitive))
        return false;
    if ( ! getFileName().isEmpty()
         && getFileName().compare(li.getFileName(),
                                    Qt::CaseInsensitive))
        return false;
    if ( ! getFunctionName().isEmpty()
         && getFunctionName().compare(li.getFunctionName(),
                                    Qt::CaseInsensitive))
        return false;
    return true;
}
