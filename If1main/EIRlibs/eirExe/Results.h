#ifndef RESULTS_H
#define RESULTS_H
#include "eirExe.h"

#include <QtCore/QList>

#include "../eirBase/Severity.h"

#include "Result.h"

class EIREXESHARED_EXPORT ResultSet
{
public:
    ResultSet(void);
    void setMinSeverity(const Severity sev);
    Severity minSeverity(void) const;
    Severity maxSeverity(void) const;
    void clear(void);
    Result at(const int x) const;
    void add(const Result & r);
    void set(const int x, const Result & r);
    int size(void) const;
    void fill(const int x);
    bool is(const Severity sev) const;
    QList<Result> list(void) const;
    Result find(const ResultCode rc) const;
    Result find(const ResultId & id) const;
    Result find(const ResultInfo & ri) const;
    Result findSeverity(const Severity sev) const;
    QList<Result> findAll(const ResultCode rc) const;
    QList<Result> findAll(const ResultId & id) const;
    QList<Result> findAll(const ResultInfo & rc) const;
    QStringList toStringList(void) const;
    ResultSet & operator<< (const Result & r);

private:
    Severity min_sev;
    Severity max_sev;
    QList<Result> result_list;
};

#endif // RESULTS_H
