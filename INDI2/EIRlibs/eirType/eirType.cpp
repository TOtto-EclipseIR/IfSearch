/*! @file eirType.cpp Definitions for eirType class
*
*/
#include "eirType.h"
#include "version.h"
#include "../../../common/version.h"

#include <eirBase/eirBase.h>
#include <eirBase/BaseLog.h>

#include "DataType.h"
#include "IntBehavior.h"
#include "StringBehavior.h"

eirType * eirType::instance_p = 0;

eirType::eirType(void)
    : Module(__MODULE__, VERSIONINFO())
{
    // Our MetaTypes
    qRegisterMetaType<eirType>("eirType");

    // Prerequisistes
    eirBase::instance();

    DataType::addBehavior(QMetaType::Char, IntBehavior::instance());
    DataType::addBehavior(QMetaType::Short, IntBehavior::instance());
    DataType::addBehavior(QMetaType::Int, IntBehavior::instance());
    DataType::addBehavior(QMetaType::Long, IntBehavior::instance());
    DataType::addBehavior(QMetaType::LongLong, IntBehavior::instance());
    DataType::addBehavior(QMetaType::UChar, IntBehavior::instance());
    DataType::addBehavior(QMetaType::UShort, IntBehavior::instance());
    DataType::addBehavior(QMetaType::UInt, IntBehavior::instance());
    DataType::addBehavior(QMetaType::ULong, IntBehavior::instance());
    DataType::addBehavior(QMetaType::ULongLong, IntBehavior::instance());

    DataType::addBehavior(QMetaType::QString, StringBehavior::instance());
}

eirType * eirType::instance(void)
{
    if ( ! instance_p) instance_p = new eirType;
    BNULPOINTER(instance_p);
    return instance_p;
}

int powerOfTwo(const int n)
{
    if (0 == n) return 0;
    bool neg = n < 0;
    int a = qAbs(n);
    int p = 0;
    while (a) a >>= 1, ++p;
    a = 1 << p;
    return neg ? -a : a;
}
