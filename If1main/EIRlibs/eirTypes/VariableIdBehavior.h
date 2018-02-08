#ifndef VARIABLEIDBEHAVIOR_H
#define VARIABLEIDBEHAVIOR_H
#include "eirTypes.h"


#include "BaseIdBehavior.h"

class EIRTYPESSHARED_EXPORT VariableIdBehavior : public BaseIdBehavior
{
public:
    static VariableIdBehavior * instance(void);

private:
    VariableIdBehavior(void);
    static VariableIdBehavior * singleton;
};

#endif // VARIABLEIDBEHAVIOR_H
