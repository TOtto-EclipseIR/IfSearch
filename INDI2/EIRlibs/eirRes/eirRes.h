/*! @file eirBase.h Declarations for eirBase library
 *
  */
#ifndef EIRRES_H
#define EIRRES_H
#ifndef __MODULE__
# define __MODULE__ "eirBase"
#endif

#include "eirres_global.h"

#include <eirBase/Module.h>

/*! eirRes library
  *
  *
  */
class EIRRESSHARED_EXPORT eirRes : public Module
{

public:
    eirRes(void);

public:
    static eirRes instance;
};

#endif // EIRRES_H
