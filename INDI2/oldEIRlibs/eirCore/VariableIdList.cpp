#include "VariableIdList.h"

VariableIdList::VariableIdList(void)
{
}

VariableIdList::operator QStringList(void) const
{
    QStringList result;
    int x = 0;
    while (x < size())
        result << at(x++);
    return result;
}
