/** @file NamedEnum.h
*	Declaration of NamedEnum class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

#include <QMap>
#include <QPair>
#include <QString>
#include <QtDebug>

#include <DDTcore.h>
#ifdef DDTCORE_LIB
template <class NAMED> class Q_DECL_EXPORT NamedEnum
        #else
template <class NAMED> class NamedEnum
        #endif
{
public:
    NamedEnum(void) : valid(false), iValue(0) {}
    NamedEnum(const NamedEnum & other)
        : valid(other.valid), iValue(other.iValue) {}
    explicit NamedEnum(const char * Name, bool Valid=true)
        : valid(Valid), iValue(max()+1)
    {
        QPair<NamedEnum<NAMED>*,QString> pair;
        pair.first = this;
        pair.second = Name;
        mapIntToPair.insert(iValue, pair);
    }
    NamedEnum(const char * Name, const int Value, const bool Valid=true)
        : valid(Valid), iValue(Value)
    {
        QPair<NamedEnum<NAMED>*,QString> pair;
        pair.first = this;
        pair.second = Name;
        mapIntToPair.insert(iValue, pair);
    }

public:
    int toInt(void) const { return iValue; }
    bool isValid(void) const { return valid; }
    NamedEnum<NAMED> & operator=(const NamedEnum<NAMED> other)
    { this->valid = other.valid, this->iValue = other.iValue; return *this; }

public: // static functions
    static int size(void) { return mapIntToPair.size(); };
    static const NamedEnum<NAMED> id(const int Index)
    {
        return mapIntToPair.contains(Index)
                ? mapIntToPair.value(Index).first
                : NamedEnum<NAMED>();
    }

    static QString name(NamedEnum<NAMED> Id)
    {
        return mapIntToPair.contains(Id.toInt())
                ? mapIntToPair.value(Id.toInt()).second
                : QString();
    }

    static int max(void)
    {
        QMapIterator<int, QPair<NamedEnum<NAMED> *, QString> > it(mapIntToPair);
        it.toBack();
        return (it.hasPrevious()) ? (it.previous().value().first->iValue) : -1;
    }

    static int min(void)
    {
        QMapIterator<int, QPair<NamedEnum<NAMED> *, QString> > it(mapIntToPair);
        return (it.hasNext()) ? (it.next().value().first->iValue) : -1;
    }

    static const NamedEnum<NAMED> id(const QString & Name)
    {
        QMapIterator<int, QPair<NamedEnum<NAMED> *, QString> > it(mapIntToPair);
        while (it.hasNext())
        {
            it.next();
            if (0 == it.value().second.compare(Name, Qt::CaseInsensitive))
                return *it.value().first;
        }
        return NamedEnum<NAMED>();
    }

    static bool isValidAsArrayIndex(void)
    {
        int n = min(), z = size(), x = max();
        return 0 == n && z == x + 1;
    }

    static QString name(const int Index)
    {
        return mapIntToPair.contains(Index) ? mapIntToPair.value(Index).second : QString();
    }

    operator int(void) const { return iValue; };

private:
    bool valid;
    int iValue;
    static QMap<int, QPair<NamedEnum<NAMED> *, QString> > mapIntToPair;
    friend bool operator==(NamedEnum<NAMED> lhs, NamedEnum<NAMED> rhs)
    { return lhs.toInt() == rhs.toInt(); }
}; // class NamedEnum

#define DECLARE_NAMEDENUM_ID(NameType, Name) \
    const NameType NameType::Name(#Name);

#define DECLARE_NAMEDENUM_IDV(NameType, Name, Valid) \
    const NameType NameType::Name(#Name, Valid);

#define DECLARE_NAMEDENUM_IDN(NameType, Name, Index) \
    const NameType NameType::Name(#Name, Index);

#define DECLARE_NAMEDENUM_IDNV(NameType, Name, Index, Valid) \
    const NameType NameType::Name(#Name, Index, Valid);

#define DECLARE_NAMEDENUM_INSTANCE(NameType) \
    QMap<int, QPair<NamedEnum<NameType>*,QString> > NameType::mapIntToPair;

#define DECLARE_NAMEDENUM_CTORS(NameType) \
    private: \
    explicit NameType(const char * Name, const bool Valid=true) \
    : NamedEnum<NameType>(Name) {(void)Valid;} \
    NameType(const char * Name, const int Value, const bool Valid=true)		\
    : NamedEnum<NameType>(Name, Value, Valid) {} \
    public: \
    NameType(void) : NamedEnum<NameType>() {} \

