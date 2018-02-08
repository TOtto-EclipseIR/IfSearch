#include "LogFilter.h"

#include <QtCore/QTextStream>

#include "LogFunction.h"
#include "LogItem.h"
#include "../eirBase/BaseLog.h"

LogFilter::LogFilter(const QString & name)
    : VariableSet(name+":LogFilter")
    , checkMask_i(0)
    , minLine_i(0)
    , maxLine_i(0)
{
    DEFINE_VARIABLESET_CTORS(LOGFILTER_VARIABLESET);
}

LogFilter::LogFilter(const VariableSet & other)
    : VariableSet(other)
    , checkMask_i(0)
    , minLine_i(0)
    , maxLine_i(0)
{
    calculate();
}

void LogFilter::calculate(void)
{
    if ( ! getModule().isEmpty())       checkMask_i |= Module;
    if ( ! getClass().isEmpty())        checkMask_i |= Class;
    if ( ! getFunction().isEmpty())     checkMask_i |= Function;
    if ( ! getFileName().isEmpty())     checkMask_i |= File;
    if ( ! getFileLines().isEmpty())
    {
        checkMask_i |= Lines;
        QString s(getFileLines());
        QTextStream txs(&s, QIODevice::ReadOnly);
        txs >> minLine_i;
        txs >> maxLine_i;
    }
    report_mask.set(getReport());
    ignore_mask.set(getIgnore());
    BDUMPVAR(report_mask.mask());
    BDUMPVAR(ignore_mask.mask());
}

int LogFilter::evaluate(const LogItem & li)
{
    int check = checkMask_i;
    BDUMPVAR(check);
    int bit = Module;
    bool valid = true;
    QString cls, fn;

    if (check & (Class | Function))
    {
        LogFunction lfn(li.getFunction());
        cls = lfn.className();
        fn = lfn.functionName();
    }

    while (check)
    {
        if (check & bit)
        {
            switch (bit)
            {
            case Module:
                if ( ! li.getModule().contains(getModule(), Qt::CaseInsensitive))
                    valid = false;
                check &= ! Module;
                break;

            case Class:
                if ( ! cls.contains(getClass(), Qt::CaseInsensitive))
                    valid = false;
                check &= ! Class;
                break;

            case Function:
                if ( ! fn.contains(getFunction(), Qt::CaseInsensitive))
                    valid = false;
                check &= ! Function;
                break;

            case File:
                if ( ! li.getFileName().contains(getFileName(), Qt::CaseInsensitive))
                    valid = false;
                check &= ! File;
                break;

            case Lines:
                {
                int ln = li.getFileLine();
                if (ln < minLine_i || ln >= maxLine_i || maxLine_i == 0)
                    valid = false;
                }
                check &= ! Lines;
                break;
            }
        }
    }

    if (valid)
    {
        Severity sev = li.getSeverity();
        if (report_mask.is(sev))  return +1;
        if (ignore_mask.is(sev))  return -1;
    }
    return 0;
}
