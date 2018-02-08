#ifndef VARIABLEIDBEHAVIOR_H
#define VARIABLEIDBEHAVIOR_H
#include "eirVariable.h"


#include "../eirKeyId/BaseIdBehavior.h"

class EIRVARIABLESHARED_EXPORT VariableIdBehavior : public BaseIdBehavior
{
public:
    static VariableIdBehavior * instance(void);

private:
    VariableIdBehavior(void);
    static VariableIdBehavior * singleton;
};

#endif // VARIABLEIDBEHAVIOR_H
