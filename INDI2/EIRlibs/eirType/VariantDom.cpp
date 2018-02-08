#include "VariantDom.h"

#include <QDomElement>

#include "DataType.h"

VariantDom::VariantDom(const QDomElement & de)
{
    QString elementTag = de.tagName();
    if (DataType::elementTag_s == elementTag)
    {
        QString dataType = de.attribute(DataType::attribute_s);
        DataType dt(dataType);
        if (dt.isValid())
            QVariant::setValue(dt.variantDom(dt, de));
    }
}

VariantDom::operator QVariant (void)
{
    return QVariant(*this);
}
