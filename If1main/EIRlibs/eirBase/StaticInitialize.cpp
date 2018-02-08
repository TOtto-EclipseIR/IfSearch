#include "StaticInitialize.h"

StaticInitialize::StaticInitialize()
{
    if ( ! staticInitialized_bool)
        if (canStaticInitialize())
            staticInitialized_bool = staticInitialize();
}

bool StaticInitialize::canStaticInitialize(void)
{
    return true;
}

bool StaticInitialize::isStaticInitialized(void)
{
    return staticInitialized_bool;
}

bool StaticInitialize::staticInitialize(void)
{
    return true;
}





