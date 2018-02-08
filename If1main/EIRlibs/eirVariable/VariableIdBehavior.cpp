#include "VariableIdBehavior.h"

VariableIdBehavior * VariableIdBehavior::singleton=0;

VariableIdBehavior * VariableIdBehavior::instance(void)
{
    if ( ! singleton)
        singleton = new VariableIdBehavior;
    return singleton;
}

VariableIdBehavior::VariableIdBehavior()
    : BaseIdBehavior(BaseIdBehavior::lower, "/")
{
    replace("_", "/");
}
