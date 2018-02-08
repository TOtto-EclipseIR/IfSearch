#ifndef LOGFILTERSET_H
#define LOGFILTERSET_H

#include "LogFilter.h"

#include <eirBase/Enumeration.h>
#define LOGFILTERKEYWORDS_ENUM(NV) \
    NV(Fatal, = 1)  \
    NV(Global,)   \
    NV(File,)   \
    NV(Function,)   \
    NV(Class,)   \
    NV(Module,)   \
    NV(Options,) \

class LogFilterKeyword : public Enumeration
{
    DECLARE_ENUMERATION(LogFilterKeyword, LOGFILTERKEYWORDS_ENUM)
};

class LogFilterSet
{
public:
    LogFilterSet(void);
    void clear(void);
    bool isFatal(const Severity sev) const;
    bool set(const QUrlQuery & query);
    bool evaluate(const LogItem & li) const;

private:
    Severity fatal_sev;
    QList<LogFilter> _filter_list;

};

#endif // LOGFILTERSET_H
