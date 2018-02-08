/*! @file eirVar.h Declarations for eirBase library
 *
  */
#ifndef EIRVAR_H
#define EIRVAR_H
#ifndef __MODULE__
# define __MODULE__ "eirVar"
#endif

#include "eirVar_global.h"

#include <eirBase/Module.h>

/*! eirVar library houses the VariableSet class and support classes
  *
  * The following classes are included:
  *
  * Variable contains an id, type, current, and default values.
  *
  * DefaultVariable contains an id, type, and default value.
  *
  * VariableTableColumns contains a list of DefaultVariable instances
  *
  * VariableMap is a map of sortable ids to Variable instances
  *
  * VariableTable contains a VariableTableColumns list plus
  *     a matrix of current values.
  *
  * VariableSet is a marriage of a VariableMap, a list of values,
  *     and binary storage identified by a string and numeric identiers
  *
  */
class EIRVARSHARED_EXPORT eirVar  : public Module
{

public:
    eirVar(void);

public:
    static eirVar * instance(void);

public:
    static eirVar * instance_p;
};

#endif // EIRVAR_H
