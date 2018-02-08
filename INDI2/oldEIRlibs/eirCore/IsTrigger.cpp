#include "IsTrigger.h"

#include <QtCore/QVariant>

#include "../eirBase/BaseLog.h"

IsTrigger::IsTrigger(const QVariant value)
    : truth(false)
{
    if (value.canConvert(QVariant::Bool))
        truth = value.toBool();
    else
        BWARNING("IsTrigger: unhandled QVariant type %1 %2",
                 value.type(), value.typeName());
}

IsTrigger::operator bool(void) const
{
    return truth;
}
