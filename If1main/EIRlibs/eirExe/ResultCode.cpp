#include "ResultCode.h"

#include "../eirBase/Severity.h"

#include "ResultRegistry.h"
#include "ResultInfo.h"

ResultCode::ResultCode(void)
{
}

ResultCode::ResultCode(const quint64 value)
    : EightCC(value)
{
}

ResultCode::ResultCode(const char *pc)
    : EightCC(pc)
{
}
