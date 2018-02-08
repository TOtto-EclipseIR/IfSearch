#include "VariableId.h"
#include "VariableIdBehavior.h"

VariableId::VariableId(const QString & inString)
    : AbstractIdString(VariableIdBehavior::instance())
{
    set(inString);
}

VariableId::VariableId(const char * chars)
    : AbstractIdString(VariableIdBehavior::instance())
{
    set(QString(chars));
}
