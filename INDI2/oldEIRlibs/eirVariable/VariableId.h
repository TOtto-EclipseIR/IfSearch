#ifndef VARIABLEID_H
#define VARIABLEID_H
#include "eirVariable.h"


#include <QList>

#include "../eirKeyId/AbstractIdString.h"

class EIRVARIABLESHARED_EXPORT VariableId : public AbstractIdString
{
public:
    VariableId(const QString & inString=QString());
    VariableId(const char * chars);
};

typedef QList<VariableId> VariableIdList;

#endif // VARIABLEID_H
