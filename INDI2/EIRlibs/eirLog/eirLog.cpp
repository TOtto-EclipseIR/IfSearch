/*! @file eirType.cpp Definitions for eirType class
*
*/
#include "eirLog.h"

#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>

#include "Logger.h"

eirLog * eirLog::instance_p = 0;

eirLog::eirLog(void)
    : Module(__MODULE__, VERSIONINFO())
{
    qRegisterMetaType<eirLog>("eirLog");

    // Prerequisites
    eirBase::instance();
    eirType::instance();

    Logger::instance_p = new Logger;
}


eirLog * eirLog::instance(void)
{
    if ( ! instance_p) instance_p = new eirLog;
    BNULPOINTER(instance_p);
    return instance_p;
}
