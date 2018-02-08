/*! @file Readable.cpp Definitions for Readable class
*
*/
#include "Sortable.h"

#include <QVariant>

#include "DataType.h"

Sortable::Sortable(const QVariant & var)
{
    DataType typ(var);
    QString::append(typ.sortable(var));
}

Sortable::operator QString (void)
{
    return QString(*this);
}
