#include "DomVariant.h"

#include <QDomElement>

#include "DataType.h"

DomVariant::DomVariant(const QVariant & var)
{
    DataType typ(var);
    QDomElement::operator = (typ.domVariant(var));
}

DomVariant::operator QDomElement (void)
{
    return QDomElement(*this);
}
