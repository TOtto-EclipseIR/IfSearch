#include "VariableSet.h"

#include <eirType/QQVariant.h>

DEFINE_DATAPROPS(VariableSet, VariableSetData)

VariableSet::VariableSet(const KeyClass keyClass,
                         const VariableId & id,
                         const QString name)
    : data(new VariableSetData)
{
    ctor();
    setKey(UniversalKey(keyClass));
    setId(id);
    setName(name);
}

void VariableSet::ctor(void)
{
    setTimeStamp(MillisecondTime::current());
}

void VariableSet::dtor(void) {}

void VariableSet::clear(void)
{
    ptrMap()->clear();
    ptrList()->clear();
    ptrBinary()->clear();
}

int VariableSet::binarySize(void) const
{
    return ptrBinary()->size();
}

int VariableSet::listSize(void) const
{
    return ptrList()->size();
}

int VariableSet::mapSize(void) const
{
    return ptrMap()->size();
}

bool VariableSet::contains(const VariableId & vid) const
{
    return ptrMap()->contains(Sortable(vid));
}

void VariableSet::set(const QQVariantList & vl)
{
    *ptrList() = vl;
}

void VariableSet::set(const Variable & vbl)
{
    ptrMap()->insert(vbl);
}

void VariableSet::set(const VariableId & vid,
                      const QQVariant &value)
{
    VariableMap * pm = ptrMap();
    VariableId sort(Sortable(vid));
    QQVariant def(pm->value(vid).getDefault());
    pm->insert(Variable(vid, value, def));
}

void VariableSet::set(const int index,
                      const QQVariant & value)
{
    QQVariantList * pl = ptrList();
    while (pl->size() <= index)
        pl->append(QQVariant());
    pl->append(value);
}

void VariableSet::set(const QByteArray & ba)
{
    QByteArray * pb = ptrBinary();
    pb->clear();
    pb->append(ba);
}

void VariableSet::reset(const VariableId & vid)
{
    VariableMap * pm = ptrMap();
    if (pm->contains(Sortable(vid)))
        (*pm)[Sortable(vid)].reset();
}

void VariableSet::append(const QQVariant & value)
{
    ptrList()->append(value);
}

Variable VariableSet::at(const VariableId & id) const
{
    return ptrMap()->value(Sortable(id));
}

QQVariant VariableSet::value(const VariableId & vid,
                            const QQVariant &defaultValue) const
{
#if 1 // def QT_DEBUG
    if (ptrMap()->contains(vid))
    {
        Variable vbl(ptrMap()->value(vid));
        QQVariant var(vbl.getCurrent());
        return var;
    }
    else
        return defaultValue;
#else
    return ptrMap()->value(Sortable(id)).var();
#endif
}

QQVariant VariableSet::value(const int index) const
{
    return (index >= 0 && index < ptrList()->size())
            ? ptrList()->at(index)
            : QQVariant();
}

QByteArray VariableSet::value(void) const
{
    return getBinary();
}

QQVariantList VariableSet::values(void) const
{
    QQVariantList qqvl;
    VariableIdList vil = ptrMap()->keys();\
    foreach (VariableId vid, vil)
        qqvl.append(ptrMap()->value(vid).getCurrent());
    return qqvl;
}


VariableSet VariableSet::extract(const VariableId & sectionId) const
{
    VariableSet result;
    int n = sectionId.sectionCount();
    foreach (VariableId vid, ids(sectionId))
    {
        VariableId newId(vid.sections(n));
        result.set(newId, value(vid));
    }
    return result;
}

VariableIdList VariableSet::ids(const VariableId & within) const
{
    VariableIdList result;
    if (within.isNull())
        foreach (Variable vbl, ptrMap()->values())
            result.append(vbl.id());
    else
    {
        QString prefix(within);
        foreach (Variable vbl, ptrMap()->values())
        {
            VariableId vid = vbl.id();
            if (QString(vid).startsWith(QString(prefix), Qt::CaseInsensitive))
                result.append(vid);
        }
    }
    return result;
}

VariableIdList VariableSet::sectionIds(const VariableId & within) const
{
    VariableIdList result;
    if (within.isNull())
        foreach (Variable vbl, ptrMap()->values())
        {
            QString section(vbl.id().section(0));
            if ( ! result.contains(VariableId(section)))
                result.append(VariableId(section));
        }
    else
    {
        int n = within.sectionCount();
        foreach (Variable vbl, ptrMap()->values())
        {
            QString section(vbl.id().section(n));
            if (QString(within) == vbl.id().sections(0, n-1))
                if ( ! result.contains(VariableId(section)))
                    result.append(VariableId(section));
        }
    }
    return result;
}

void VariableSet::implant(const VariableSet & other,
                          const VariableId & sectionId)
{
    foreach (Variable vbl, other.ptrMap()->values())
    {
        VariableId vid(vbl.id());
        vid.prepend(sectionId);
        set(vid, vbl.getCurrent());
    }
}

QList<Variable> VariableSet::all(void) const
{
    return ptrMap()->values();
}
