#ifndef VARIABLEID_H
#define VARIABLEID_H
#include "eirTypes.h"


#include <QList>

#include "AbstractIdString.h"

class EIRTYPESSHARED_EXPORT VariableId : public AbstractIdString
{
public:
    VariableId(const QString & inString=QString());
    VariableId(const char * chars);
};

typedef QList<VariableId> VariableIdList;

#endif // VARIABLEID_H
