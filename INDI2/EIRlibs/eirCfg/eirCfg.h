/*! @file eirCfg.h Declarations for eirCfg class
*
*/
#ifndef EIRCFG_H
#define EIRCFG_H
#include "eircfg_global.h"

#ifndef __MODULE__
# define __MODULE__ "eirCfg"
#endif

#include <eirBase/Module.h>

class EIRCFGSHARED_EXPORT eirCfg : public Module
{

public:
    eirCfg(void);

public:
    static eirCfg * instance(void);

public:
    static eirCfg * instance_p;
};

#endif // EIRCFG_H
