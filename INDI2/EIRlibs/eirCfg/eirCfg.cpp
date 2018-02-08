/*! @file eirCfg.cpp Definitions for eirCfg class
*
*/
#include "eirCfg.h"

#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include <eirType/eirType.h>
#include <eirVar/eirVar.h>
#include <eirLog/eirLog.h>

eirCfg * eirCfg::instance_p = 0;

eirCfg::eirCfg(void)
    : Module(__MODULE__, VERSIONINFO())
{
    // Prequisites
    eirBase::instance();
    eirType::instance();
    eirLog::instance();
    eirVar::instance();
}


eirCfg * eirCfg::instance(void)
{
    if ( ! instance_p) instance_p = new eirCfg;
    BNULPOINTER(instance_p);
    return instance_p;
}
