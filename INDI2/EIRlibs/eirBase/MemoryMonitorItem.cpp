/*! @file MemoryMonitorItem.h Definitions for MemoryMonitorItem class
*
*/
#include "MemoryMonitorItem.h"

#include <QSharedData>

#include "BaseLog.h"

MemoryMonitorItem::MemoryMonitorItem()
    : data(new MemoryMonitorData)
{
}

MemoryMonitorItem::MemoryMonitorItem(const MemoryMonitorItem &rhs)
    : data(rhs.data)
{
}

MemoryMonitorItem & MemoryMonitorItem::operator = (const MemoryMonitorItem &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

MemoryMonitorItem::~MemoryMonitorItem()
{
}

bool MemoryMonitorItem::isNull(void) const
{
    return ! getPointer();
}

void MemoryMonitorItem::setInitial(bool on)
{
    int f = getFlags();
    if (on)
        f |= isInitial;
    else
        f &= ~ isInitial;
}
