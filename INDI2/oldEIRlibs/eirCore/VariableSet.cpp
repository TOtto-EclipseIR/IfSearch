#include "VariableSet.h"

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include "../eirBase/BaseLog.h"
#include "VariableIdList.h"

VariableSetData::VariableSetData(const VariableSetData & other)
    : QSharedData(other)
    , name_s(other.name_s)
    , key_u64(other.key_u64)
    , id_s(other.id_s)
    , vbl_map(other.vbl_map)
    , var_list(other.var_list)
    , _ba(other._ba)
{
}


VariableSet::VariableSet(const QString & name)
    : data(new VariableSetData)
{
    data->name_s = name;
}

VariableSet::VariableSet(const quint64 key,
            const QString & id,
            const QString & name)
    : data(new VariableSetData)
{
    data->name_s = name;
    data->key_u64 = key;
    data->id_s = id;
}

VariableSet::VariableSet(const VariableSet & other)
    : data(other.data)
{
}

VariableSet &VariableSet::operator=(const VariableSet & rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

VariableSet::~VariableSet()
{
}

QString VariableSet::name(void) const
{
    return data->name_s;
}

quint64 VariableSet::key(void) const
{
    return data->key_u64;
}

QString VariableSet::id(void) const
{
    return data->id_s;
}

void VariableSet::setName(const QString & newName)
{
    data->name_s = newName;
}

void VariableSet::setKey(const quint64 key)
{
    data->key_u64 = key;
}

void VariableSet::setId(const QString & id)
{
    data->id_s = id;
}

void VariableSet::clear(void)
{
    data->vbl_map.clear();
    data->var_list.clear();
    data->_ba.clear();
}

int VariableSet::binarySize(void) const
{
    return data->_ba.size();
}

int VariableSet::listSize(void) const
{
    return data->var_list.size();
}

int VariableSet::mapSize(void) const
{
    return data->vbl_map.size();
}

bool VariableSet::contains(const VariableId & vid) const
{
    return data->vbl_map.contains(vid.sortable());
}

void VariableSet::reset(void)
{
    foreach (QString key, data->vbl_map.keys())
        data->vbl_map[key].reset();
}

/** @fn blog()
  *
  * @todo Stringify QVariants
  */
void VariableSet::blog(void) const
{
#if 0
    BLOG(Severity::Dump, "VariableSet: name=%s, key=%d 0x%X, id=%s",
         qPrintable(data->name_s), data->key_u64,
         data->key_u64, qPrintable(data->id_s));
    foreach (QString key, data->vbl_map.keys())
    {
        Variable vbl(data->vbl_map.value(key));
        BLOG(Severity::Dump, "%s = {%s}",
             qPrintable(vbl.id()), qPrintable(vbl.var().toString()));
    }
    for (int x = 0; x < data->var_list.size(); ++x)
        BLOG(Severity::Dump, "%i. {%s}", x,
             qPrintable(data->var_list.at(x).toString()));
    BLOG(Severity::Dump, "%d bytes of Binary", data->_ba.size());
#endif
}


bool VariableSet::isEmpty(void) const
{
    return data->vbl_map.isEmpty()
                && data->var_list.isEmpty()
                && data->_ba.isEmpty();
}

void VariableSet::set(const QVariantList & vl)
{
    data->var_list = vl;
}

void VariableSet::set(const Variable & vbl)
{
    //data->vbl_map.insert(vbl.id().sortable(), vbl);
    data->vbl_map[vbl.id().sortable()] = vbl;
}

void VariableSet::set(const VariableId & vid,
                      const QVariant & value)
{
    //data->vbl_map.insert(vid.sortable(), Variable(vid, value));
    QVariant def(data->vbl_map[vid.sortable()].defaultVar());
    data->vbl_map[vid.sortable()] = Variable(vid, value, def);
}

void VariableSet::set(const int index,
                      const QVariant & value)
{
    while (data->var_list.size() <= index)
        data->var_list.append(QVariant());
    data->var_list[index] = value;
}

void VariableSet::set(const QByteArray & ba)
{
    data->_ba = ba;
}

void VariableSet::reset(const VariableId & id)
{
    if (data->vbl_map.contains(id.sortable()))
        data->vbl_map[id.sortable()].reset();
}

void VariableSet::append(const QVariant & value)
{
    data->var_list.append(value);
}

Variable VariableSet::at(const VariableId & id) const
{
    return data->vbl_map.value(id.sortable());
}

QVariant VariableSet::value(const VariableId & id,
                            const QVariant &defaultValue) const
{
#if 1 // def QT_DEBUG
    QString key(id.sortable());
    if (data->vbl_map.contains(key))
    {
        Variable vbl(data->vbl_map.value(key));
        QVariant var(vbl.var());
        return var;
    }
    else
        return defaultValue;
#else
    return data->vbl_map.value(id.sortable()).var();
#endif
}

QVariant VariableSet::value(const int index) const
{
    return (index >= 0 && index < data->var_list.size())
            ? data->var_list.at(index)
            : QVariant();
}

QByteArray VariableSet::value(void) const
{
    return data->_ba;
}

QVariantList VariableSet::values(void) const
{
    return data->var_list;
}


VariableSet VariableSet::exportSection(const VariableId & sectionId) const
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
        foreach (Variable vbl, data->vbl_map.values())
            result.append(vbl.id());
    else
    {
        QString prefix(within.sortable());
        foreach (Variable vbl, data->vbl_map.values())
            if (vbl.id().sortable().startsWith(prefix))
                result.append(vbl.id());
    }
    return result;
}

VariableIdList VariableSet::sectionIds(const VariableId & within) const
{
    VariableIdList result;
    if (within.isNull())
        foreach (Variable vbl, data->vbl_map.values())
        {
            QString section(vbl.id().section(0));
            if ( ! result.contains(VariableId(section)))
                result.append(VariableId(section));
        }
    else
    {
        int n = within.sectionCount();
        foreach (Variable vbl, data->vbl_map.values())
        {
            QString section(vbl.id().section(n));
            if (QString(within) == vbl.id().sections(0, n-1))
                if ( ! result.contains(VariableId(section)))
                    result.append(VariableId(section));
        }
    }
    return result;
}

void VariableSet::import(const VariableSet & other,
                         const VariableId & sectionId)
{
    foreach (Variable vbl, other.data->vbl_map.values())
    {
        VariableId vid(vbl.id());
        vid.prepend(sectionId);
        set(vid, vbl.var());
    }
}

QList<Variable> VariableSet::all(void) const
{
    return data->vbl_map.values();
}
