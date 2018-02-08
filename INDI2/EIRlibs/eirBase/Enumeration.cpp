/*! @file Enumeration.cpp Definitions for Enumeration base class
*/

#include "Enumeration.h"
#include "BaseLog.h"

#include <QStringList>

/*! Enumeration Construct an empty, invalid item */
Enumeration::Enumeration(void)
    : value_i(0)
    , name_s("[null]")
    , name_map(0)
{
}

/*! Construct an item from a valid integer

    @arg    const int value a valid integer
    @note   if the specified value is not valid, an invalid item is constructed

*/
Enumeration::Enumeration(Named<int> * p,
                         const int value)
    : value_i(value)
    , name_map(p)
{
}

Enumeration::Enumeration(Named<int> * p,
                         const QString & name)
    : value_i(0)
    , name_map(p)
{
    set(name);
}

Enumeration::Enumeration(const Enumeration & other)
    : value_i(other.value_i)
    , name_s(other.name_s)
    , name_map(other.name_map)
{
}

Enumeration & Enumeration::operator = (const Enumeration & other)
{
    value_i = other.value_i, name_s = other.name_s, name_map = other.name_map;
    BFUNCTION(value_i, name_s);
    return *this;
}

bool Enumeration::isEmpty(void) const
{
    return name_map ? name_map->isEmpty() : true;
}

Enumeration Enumeration::first(void) const
{
    BFUNCTION();
    BFNRETURN((isEmpty() ? Enumeration() : Enumeration(name_map, name_map->first(1))).name());
    return isEmpty() ? Enumeration() : Enumeration(name_map, name_map->first(1));
}

Enumeration Enumeration::last(void) const
{
    return isEmpty() ? Enumeration() : Enumeration(name_map, name_map->last(1));
}

QString Enumeration::nameOf(const int value) const
{
    BFUNCTION(value);
    QString result = name_map ? name_map->value(value) : QString();
    BFNRETURN(result);
    return result;
}

int Enumeration::valueOf(const QString & name) const
{
    BFUNCTION(name);
    int result = name_map ? name_map->value(name) : 0;
    BFNRETURN(result);
    return result;
}

Enumeration & Enumeration::operator ++(void) // prefix ++e
{
    BFUNCTION();
    if (name_map)
        set(name_map->next(value_i));
    BFNRETURN(name());
    return *this;
}

Enumeration Enumeration::operator ++(int)  // postfix e++
{
    BFUNCTION();
    Enumeration before(*this);
    if (name_map)
        set(name_map->next(value_i));
    BFNRETURN(before.name());
    return before;
}

Enumeration & Enumeration::operator --(void) // prefix --e
{
    BFUNCTION();
    if (name_map)
        set(name_map->previous(value_i));
    BFNRETURN(name());
    return *this;
}

Enumeration Enumeration::operator --(int)  // postfix e--
{
    BFUNCTION();
    Enumeration before(*this);
    if (name_map)
        set(name_map->previous(value_i));
    BFNRETURN(before.name());
    return before;
}

void Enumeration::set(const int value)
{
    if (name_map && name_map->contains(value))
    {
        value_i = value;
        name_s = name_map->value(value_i);
    }
    else
        name_map = 0;

    if ( ! name_map)
    {
        value_i = 0;
        name_s.clear();
    }
}

void Enumeration::set(const QString & name)
{
    if (name_map && name_map->contains(name))
    {
        value_i = name_map->value(name);
        name_s = name_map->value(value_i);
    }
    else
        name_map = 0;

    if ( ! name_map)
    {
        value_i = 0;
        name_s.clear();
    }
}

void Enumeration::setInvalid(void)
{
    BFUNCTION();
    value_i = 0, name_s.clear(), name_map = 0;
}

bool Enumeration::isValid(void) const
{
    BFUNCTION();
    bool result = name_map
            && (name_map->contains(value_i)
                || 0 == value_i);
    BFNRETURN(result);
    return result;
}

bool Enumeration::isNull(void) const
{
    BFUNCTION();
    bool result = ! name_map;
    BFNRETURN(result);
    return result;
}

bool Enumeration::is(const int value) const
{
    BFUNCTION(value);
    bool result = isValid() && value_i == value;
    BFNRETURN(result);
    return result;
}

bool Enumeration::operator == (const Enumeration & other) const
{
    BFUNCTION(other.value(), other.name());
    bool result = isValid()
            && other.isValid()
            && value_i == other.value_i;
    BFNRETURN(result);
    return result;
}

bool Enumeration::operator < (const Enumeration & other) const
{
    BFUNCTION(other.value(), other.name());
    bool result = isValid()
            && other.isValid()
            && value_i < other.value_i;
    BFNRETURN(result);
    return result;
}

Enumeration::operator int (void) const
{
    return value();
}

int Enumeration::value(void) const
{
    return value_i;
}

QString Enumeration::name(void) const
{
    BFUNCTION();
    BFNRETURN(name_s);
    return name_s;
}

QList<Enumeration> Enumeration::all(void)
{
    BFUNCTION();
    QList<Enumeration> result;
    foreach (int i, values())
        result.append(Enumeration(name_map, i));
    BFNRETURN(result.size());
    return result;
}

QList<int> Enumeration::values(void) const
{
    BFUNCTION();
    QList<int> result = name_map ? name_map->keys() : QList<int>();
    BFNRETURN(result.size());
    return result;
}

QStringList Enumeration::nameFlags(int f)
{
    BFUNCTION();
    QStringList nameList;
    if (name_map)
    {
        QList<int> intList = name_map->keys();
        foreach (int i, intList)
            if (i && i == (i & f))
                nameList << name_map->name(i);
    }
    BFNRETURN(nameList.size());
    return nameList;
}

bool Enumeration::isValid(const QString & name)
{
    return (name_map && name_map->contains(name));
}

int Enumeration::value(const QString & name)
{
    BFUNCTION(name);
    int result = -1;
    if (name_map)
    {
        if (name_map->contains(name))
            result = name_map->value(name);
        else
            BWARNING("Enumeration::value(%s) invalid", name);
    }
    else
       BWARNING("Enumeration::value(%s) with no instance", name);
    BFNRETURN(result);
    return result;
}

QList<Enumeration> Enumeration::parse(const QString & string)
{
    BFUNCTION(string);
    QList<Enumeration> result;

    foreach (QString s, string.simplified()
                              .split(' ', QString::SkipEmptyParts))
    {
        bool negate = false;
        bool toggle = false;
        if (s.startsWith('-'))
        {
            negate = true;
            s = s.mid(1);
        }
        else if (s.startsWith('!'))
        {
            toggle = true;
            s = s.mid(1);
        }

        if ("~" == s)
        {
            if (negate)
                result.clear();
            else
                result = all();
        }
        else if (s.contains('~'))
        {
            BASSERT(isValid());
            Enumeration min_v(first());
            Enumeration max_v(last());
            if (s.startsWith('~'))
                max_v.set(s.mid(1));
            else if (s.endsWith('~'))
                min_v.set(s.left(s.size()-1));
            else
            {
                int x = s.indexOf('~');
                min_v.set(s.left(x));
                max_v.set(s.mid(x+1));
            }
            if (min_v.isValid() && max_v.isValid())
            {
                BASSERT(isValid());
                Enumeration v(min_v);
                do
                {
                    if (negate)
                        result.removeAll(v);
                    else
                        result.append(v);
                    ++v;
                } while (v.isValid() && v <= max_v);
            }
        }
        else
        {
            BASSERT(isValid());
            Enumeration v(first());
            v.set(s);
            if (v.isValid())
            {
                if (negate)
                    result.removeAll(v);
                else if (toggle)
                {
                    if (result.contains(v))
                        result.removeAll(v);
                    else
                        result.append(v);
                }
                else
                    result.append(v);
            }
            else
                BWARNING("Enumeration(%s) invalid", s);
        }
    }
    BFNRETURN(result.size());
    return result;
}

int Enumeration::parseFlags(const QString & string)
{
    int result = 0;
    QList<Enumeration> eList = parse(string);
    foreach (Enumeration e, eList)
        result |= e.value();
    return result;
}
