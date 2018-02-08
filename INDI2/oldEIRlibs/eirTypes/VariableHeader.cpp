#include "VariableHeader.h"

#include "VariableGroup.h"
#include "VariableType.h"

VariableHeader::VariableHeader(void)
{
}

VariableHeader::VariableHeader(VariableGroup * vg)
{
    foreach (VariableId vid, vg->ids())
    {
        Variable v = vg->at(vid);
        VariableColumn vc(vid, v.variableType());
        column_list.append(vc);;
    }
}

bool VariableHeader::isNull(void) const
{
    return column_list.isEmpty();
}


int VariableHeader::size(void) const
{
    return column_list.size();
}

VariableId VariableHeader::id(int index) const
{
    return column_list.value(index).first;
}

QMetaType::Type VariableHeader::metaType(int index) const
{
    return column_list.value(index).second->metaType();
}

QList<VariableColumn> VariableHeader::header(void) const
{
    return column_list;
}
