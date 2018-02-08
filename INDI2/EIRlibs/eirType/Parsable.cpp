/*! @file Parsable.cpp Definitions for Parsable class
*
*/
#include "Parsable.h"

#include <QVariant>

#include "DataType.h"

Parsable::Parsable(const QVariant & var)
{
    DataType dt(var);
    QString::append(dt.parsable(var));
}

Parsable::operator QString (void)
{
    return QString(*this);
}
