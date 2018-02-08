#include "UniversalKey.h"

#include "../eirBase/KeyClass.h"
#include "../eirBase/KeyClassBehavior.h"

const qint64 UniversalKey::sizeMask_i64 = 0x0000000000000003LL;

UniversalKey::UniversalKey(const qint64 i)
    : _i64(i)
    , behavior(0)
{
}
