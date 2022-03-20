#ifndef VARIABLEID_H
#define VARIABLEID_H
#include "eirCore.h"

#include "../eirBase/AbstractIdString.h"

class VariableId : public AbstractIdString
{
public:
    VariableId(const QString & inString=QString());
    VariableId(const char * chars);
    VariableId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);
    VariableId(const VariableId & prefix,
               const char * suffix);
};

#endif // VARIABLEID_H
