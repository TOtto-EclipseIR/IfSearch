/*! @file eirExe.cpp Definitions for eirExe class
*
*/
#include "eirExe.h"

#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>
#include <eirVar/eirVar.h>
#include <eirLog/eirLog.h>
#include <eirCfg/eirCfg.h>

eirExe * eirExe::instance_p = 0;

eirExe::eirExe(void)
    : Module(__MODULE__, VERSIONINFO())
{
    // Prequisites
    eirBase::instance();
    eirType::instance();
    eirLog::instance();
    eirVar::instance();
    eirCfg::instance();
}

eirExe * eirExe::instance(void)
{
    if ( ! instance_p) instance_p = new eirExe;
    BNULPOINTER(instance_p);
    return instance_p;
}
