#include "Results.h"

#include <QtCore/QStringList>

#include "Log.h"
#include "Result.h"
#include "ResultRegistry.h"

ResultSet::ResultSet(void)
    : min_sev(Severity::Minimum)
{
    clear();
}

void ResultSet::setMinSeverity(const Severity sev)
{
    min_sev = sev;
}

Severity ResultSet::minSeverity(void) const
{
    return min_sev;
}

Severity ResultSet::maxSeverity(void) const
{
    return max_sev;
}

void ResultSet::clear(void)
{
    result_list.clear();
    max_sev = Severity::Minimum;
}

Result ResultSet::at(const int x) const
{
    return (x >= 0 && x < result_list.size())
            ? result_list.at(x) : Result();
}

void ResultSet::add(const Result & r)
{
    if (r.severity() >= min_sev)
    {
        if (r.severity() > max_sev) max_sev = r.severity();
        result_list.append(r);
    }
}

void ResultSet::set(const int x, const Result & r)
{
    while (result_list.size() <= x) result_list.append(Result());
    if (r.severity() > max_sev) max_sev = r.severity();
    result_list[x] = r;
}

ResultSet & ResultSet::operator << (const Result & r)
{
    add(r);
    return *this;
}

int ResultSet::size(void) const
{
    return result_list.size();
}

void ResultSet::fill(const int x)
{
    while (result_list.size() < x)
        result_list.append(Result());
}

bool ResultSet::is(const Severity sev) const
{
    FUNCTION(sev.name());
    bool b = max_sev >= sev;
    FNRETURN(b);
    return b;
}

QList<Result> ResultSet::list(void) const
{
    return result_list;
}

Result ResultSet::find(const ResultCode rc) const
{
    if (rc.isNull())   return Result();
    foreach (Result r, result_list)
        if (r.code() == rc)
            return r;
    return Result();
}

Result ResultSet::find(const ResultId & id) const
{
    return find(ResultRegistry::code(id));
}


Result ResultSet::findSeverity(const Severity sev) const
{
    if (! sev.isValid())   return Result();
    foreach (Result r, result_list)
        if (r.severity() == sev)
            return r;
    return Result();
}

QList<Result> ResultSet::findAll(const ResultCode rc) const
{
    QList<Result> resultList;
    if (rc.isNull())   return resultList;
    foreach (Result r, result_list)
        if (r.code() == rc)
            resultList << r;
    return resultList;
}

QList<Result> ResultSet::findAll(const ResultId & id) const
{
    return findAll(ResultRegistry::code(id));
}

QStringList ResultSet::toStringList(void) const
{
    QStringList result;
    foreach (Result r, result_list)
        result.append(r.toString());
    return result;
}
