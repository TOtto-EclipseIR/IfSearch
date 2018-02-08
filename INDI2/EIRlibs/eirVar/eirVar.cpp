/*! @file eirVar.cpp Definitions for eirVar library
  *
  */
#include "eirVar.h"

#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>
#include <eirType/DataType.h>
#include <eirType/StringBehavior.h>

eirVar * eirVar::instance_p;

eirVar::eirVar(void)
    : Module(__MODULE__, VERSIONINFO())
{
    // Prequisites
    eirBase::instance();
    eirType::instance();

    // Our behaviors
    DataType::addBehavior("VariableId",
                          StringBehavior::instance());
}

eirVar * eirVar::instance(void)
{
    if ( ! instance_p) instance_p = new eirVar;
    BNULPOINTER(instance_p);
    return instance_p;
}
