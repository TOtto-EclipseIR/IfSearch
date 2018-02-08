/*! @file eirKID.h Declaration of the eirKID class
  *
  */
#ifndef EIRKID_H
#define EIRKID_H
#ifndef __MODULE__
# define __MODULE__ "eirKID"
#endif

#include "eirKID_global.h"

#include <eirBase/Module.h>

/*! eirKID library provides base key and id classes and for their management.
  *
  */
class EIRKIDSHARED_EXPORT eirKID : public Module
{
public:
    eirKID(void);

public:
    static eirKID * instance(void);

public:
    static eirKID * instance_p;
};

#endif // EIRKID_H
