/** @file NamedArray.h
*	Declaration of NamedArray template class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <QDomElement>
#include <QVector>
#include <QString>
#include <QtDebug>

#include <DDTcore.h>
#include <NamedEnum.h>

template <class NAMED, class T> class NamedArray : public QVector<T>
{
public:
    NamedArray(void)
    {
        fill(T(), NAMED::size());
    }

    bool isValidIndex(const int Index) const
    {
        return ((Index >= 0) && (Index < size()));
    }

    void merge(const NamedArray & other)
    {
        for (int i = 0; i < NAMED::size(); ++i)
            if (T() == get(i))
                set(i, other.get(i));
    }

    T get(const NAMED Id) const
    {
        return Id.isValid() ? at(Id.toInt()) : T();
    }

    T get(const int Index) const
    {
        return isValidIndex(Index) ? at(Index) : T();
    }

    void set(const NAMED Id, const T & Value)
    {
        if (Id.isValid())
            replace(Id.toInt(), Value);
    }

    void set(const int Index, const T & Value)
    {
        if (isValidIndex(Index))
            replace(Index, Value);
    }

    bool fillDomElement(QDomElement * element) const
    {
        bool err = false;
        for (int f = NAMED::min(); f <= NAMED::max(); f++)
            err |= ! DDTcore::appendDomElement(element, NAMED::name(f), get(f));
        return err;
    }

    void fromDomElement(const QDomElement & elem, const QString & childName)
    {
        QDomElement elemVar = elem.firstChildElement(childName);
        while ( ! elemVar.isNull())
        {
            T v;
            QString name = elemVar.attribute("Name");
            const NamedEnum<NAMED> i = NAMED::id(name);
            if (i.isValid())
            {
                int index = i.toInt();
                if (DDTcore::parseDomElement(&v, elemVar))
                    set(index, v);
            }
            elemVar = elemVar.nextSiblingElement(childName);
        }
    }

public:
}; // class NamedArray

#define DECLARE_NAMEDARRAY_INSTANCE(NameType, ArrayType) \
    template class NamedArray<NameType, ArrayType>;
