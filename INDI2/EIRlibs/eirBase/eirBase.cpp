/*! @file eirBase.cpp Definitions for eirBase library
  *
  */
#include "eirBase.h"

#include "version.h"
#include "../../../common/version.h"

#include "BaseLog.h"
#include "SeverityMask.h"

eirBase * eirBase::instance_p = 0;

/*
void allocFail(void)
{
    BMEMORY("Out of memory.");
}
*/

eirBase::eirBase(void)
    : Module(__MODULE__, VERSIONINFO())
{
    qRegisterMetaType<eirBase>("eirBase");

    environment = QProcessEnvironment::systemEnvironment();

    //std::set_new_handler(allocFail);

    BASSERT(SeverityMask::checkPrerequisites());
}


QString eirBase::envVar(const QString & name,
                        const QString & defaultValue) const
{
    return environment.value(name, defaultValue);
}


eirBase * eirBase::instance(void)
{
    if ( ! instance_p) instance_p = new eirBase;
    BNULPOINTER(instance_p);
    return instance_p;
}
