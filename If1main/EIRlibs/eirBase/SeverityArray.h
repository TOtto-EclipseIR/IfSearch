#ifndef SEVERITYARRAY_H
#define SEVERITYARRAY_H

#include <QtCore/QVector>

#include "Severity.h"

#include "BaseLog.h"

template <class T> class SeverityArray
{
public:
    SeverityArray(void) { fill(); }
    SeverityArray(const T & t) { fill(t); }

    void fill(const T & t=T())
    {
        severity_vector.clear();
        severity_vector.fill(t, Severity::Maximum+1);
    }

    void set(const T & t,
             const Severity from,
             const Severity to=Severity::Maximum)
    {
        Severity sev = from;
        do
        {
            severity_vector[sev] = t;
            //BTRACE("v[%1=%2]=%3", sev.name(), sev.value(), t);
            //qDebug() << sev.name() << t;
        }   while (++sev <= to);
    }

    void set(const Severity sev,
             const T & t)
    {
        severity_vector[sev] = t;
    }

    T at(const Severity sev) const
    {
        return severity_vector[sev];
    }

    bool isNull(const Severity sev) const
    {
        return T() == at(sev);
    }

private:
    QVector<T> severity_vector;
};

#endif // SEVERITYARRAY_H
