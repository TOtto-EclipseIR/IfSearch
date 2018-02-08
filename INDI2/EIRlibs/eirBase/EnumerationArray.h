/*! \file EnumerationArray.h Declarations for EnumerationArray template
  *
  */
#ifndef ENUMERATIONARRAY_H
#define ENUMERATIONARRAY_H

#include <QtCore/QVector>

#include "Enumeration.h"

template <class ENUM, class T> class EnumerationArray
{
public:
    EnumerationArray(void) { fill(); }
    EnumerationArray(const T & t) { fill(t); }

    void fill(const T & t=T())
    {
        ENUM e;
        _vector.clear();
        _vector.fill(t, e.last().value());
    }

    void set(const T & t,
             const ENUM from,
             const ENUM to=ENUM::last())
    {
        ENUM e = from;
        do
        {
            _vector[e] = t;
        }   while (++e <= to);
    }

    void set(const ENUM e,
             const T & t)
    {
        _vector[e] = t;
    }

    T at(const ENUM e) const
    {
        return _vector[e];
    }

    bool isNull(const ENUM e) const
    {
        return T() == at(e);
    }

private:
    QVector<T> _vector;
};

#endif // ENUMERATIONARRAY_H
