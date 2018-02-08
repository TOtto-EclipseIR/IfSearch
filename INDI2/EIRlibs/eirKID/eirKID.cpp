/*! @file eirKID.cpp Definitions for eirKID class
*
*/
#include "eirKID.h"

#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>
#include <eirType/DataType.h>
#include <eirType/StringBehavior.h>

#include "BasicId.h"
#include "BaseIdString.h"

eirKID * eirKID::instance_p = 0;

eirKID::eirKID(void)
    : Module(__MODULE__, VERSIONINFO())
{
    // Prerequisites
    eirBase::instance();
    eirType::instance();

    // Our behaviors
    DataType::addBehavior(qRegisterMetaType<BasicId>("BasicId"),
                          StringBehavior::instance());
    DataType::addBehavior(qRegisterMetaType<BaseIdString>("BaseIdString"),
                          StringBehavior::instance());
}

eirKID * eirKID::instance(void)
{
    if ( ! instance_p) instance_p = new eirKID;
    BNULPOINTER(instance_p);
    return instance_p;
}
