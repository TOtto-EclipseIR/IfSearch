#include "VariableMap.h"

VariableMap::VariableMap(void)
{
}

bool VariableMap::contains(const VariableId & id) const
{
    return contains(Sortable(id));
}

void VariableMap::insert(const Variable var)
{
    QMap<Sortable, Variable>::insert(Sortable(var.id()), var);
}

VariableIdList VariableMap::keys(void) const
{
    VariableIdList result;
    foreach (Variable var, values())
        result.append(var.id());
    return result;
}

int VariableMap::remove(const VariableId & id)
{
    return remove(Sortable(id));
}

Variable VariableMap::take(const VariableId & id)
{
    return take(Sortable(id));
}

Variable VariableMap::value(const VariableId & id,
                            const Variable defaultVar) const
{
    return value(Sortable(id), defaultVar);
}

Variable VariableMap::operator [] (const VariableId & id)
{
    return operator [] (Sortable(id));
}

const Variable VariableMap::operator [] (const VariableId & id) const
{
    return operator [] (Sortable(id));
}
