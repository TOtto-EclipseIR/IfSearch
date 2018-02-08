#include "VariableGroup.h"

#include <QtDebug>
#include <QtXml/QDomElement>

VariableGroup::VariableGroup(const VariableId & sectionId)
    : section_id(sectionId)
    , csvRows(0)
{
}
/*
VariableGroup::VariableGroup(const QDomElement & de)
{
    readXml(de);
}
*/
VariableId VariableGroup::sectionId(void) const
{
    return section_id;
}

bool VariableGroup::isEmpty(void) const
{
    return var_map.isEmpty();
}

int VariableGroup::size(void) const
{
    return var_map.size();
}

bool VariableGroup::contains(const VariableId & id) const
{
    return var_map.contains(id.sortable());
}

void VariableGroup::add(const Variable & var)
{
    VariableId vid = var.id();
    vid.prepend(section_id);
    id_list.append(vid);
    var_map.insert(vid.sortable(), var);
    //qDebug() << objectName() << vid() << "<+" << var.var();
    csvRows = qMax(csvRows, var.csvHeadingCount());
}

void VariableGroup::add(VariableGroup * other,
                        VariableId sectionId)
{
    if (sectionId.isNull()) sectionId = other->sectionId();
    foreach (Variable v, other->all())
    {
        VariableId vid(v.id());
        vid.prepend(sectionId);
        set(vid, v.var());
    }
}

Variable VariableGroup::at(const VariableId & id) const
{
    Variable v(var_map.value(id.sortable()));
    //qDebug() << id() << "@" << v.var();
    return v;
}

QVariant VariableGroup::value(const VariableId & id) const
{
    return at(id).var();
}

bool VariableGroup::set(const VariableId & id, const Variable & var)
{
    bool result = var_map.contains(id.sortable());
    if (result)
        var_map[id.sortable()] = var;
    else
        add(var);
    //qDebug() << objectName() << id() << "<-" << var.var();
    return result;
}

bool VariableGroup::set(const VariableId & id, const QVariant & var)
{
    if (var_map.contains(id.sortable()))
    {
        var_map[id.sortable()].set(var);
        //qDebug() << sectionId()() << id() << "<-" << var;
        return true;
    }
    else
    {
        add(Variable(id, var));
        return false;
    }
}

void VariableGroup::set(const VariableGroup * vars)
{
    foreach (Variable v, vars->all())
        set(v.id(), v);
}

int VariableGroup::implant(const VariableId & sectionId,
                           const VariableGroup * vars)
{
    int count = 0;
    foreach (Variable v, vars->all())
        if (v.id().section(0) == sectionId)
        {
            VariableId vid = v.id();
            QString newId = vid.sections(1);
            //qDebug() << newId << "<i" << v.var();
            set(VariableId(newId), v.var());
            ++count;
        }
    return count;
}

void VariableGroup::reset(const VariableId & id)
{
    if (var_map.contains(id.sortable()))
        var_map[id.sortable()].reset();
}

void VariableGroup::reset()
{
    foreach (QString key, var_map.keys())
        var_map[key].reset();
}

void VariableGroup::clear(void)
{
    var_map.clear();
    id_list.clear();
}

void VariableGroup::remove(const VariableId & id)
{
    var_map.remove(id.sortable());
    id_list.removeAll(id);
}

VariableIdList VariableGroup::ids(void) const
{
    return id_list;
}

QStringList VariableGroup::sectionIds(const VariableId & within)
{
    QStringList result;

    if (within.isNull())
        foreach (VariableId vid, id_list)
        {
            QString section(vid.section(0));
            if ( ! result.contains(section))
                result.append(section);
        }
    else
    {
        int n = within.sectionCount();
        foreach (VariableId vid, id_list)
        {
            QString section(vid.section(n));
            if (QString(within) == vid.sections(0, n-1))
                if ( ! result.contains(section))
                    result.append(section);
        }
    }

    return result;
}

QList<Variable> VariableGroup::all(void) const
{
    return var_map.values();
}

void VariableGroup::dump(void) const
{
    qDebug() << "Dumping" << sectionId();
    foreach (VariableId id, id_list)
    {
        Variable v = var_map.value(id.sortable());
        qDebug() << "---" << id << "=" << v.var();
    }
}
/*
QDomElement VariableGroup::writeXml(void) const
{
    QDomElement result;
    // TODO
    return result;
}

bool VariableGroup::readXml(const QDomElement & de)
{
    // TODO
    return false;
}
*/
int VariableGroup::csvHeadingRowCount(void) const
{
    return csvRows;
}
