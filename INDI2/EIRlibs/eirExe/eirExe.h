/*! @file eirExe.h Declarations for eirExe class
*
*/
#ifndef EIREXE_H
#define EIREXE_H
#include "eirexe_global.h"
#ifndef __MODULE__
# define __MODULE__ "eirCfg"
#endif

#include <eirBase/Module.h>

class EIREXESHARED_EXPORT eirExe : public Module
{
public:
    eirExe(void);

public:
    static eirExe * instance(void);

public:
    static eirExe * instance_p;
};

#endif // EIREXE_H
