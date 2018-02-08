/*! @file MemoryMonitor.h Definitions for MemoryMonitor class
*
*/
#include "MemoryMonitor.h"

#include <QThread>

#include "BaseLog.h"

DEFINE_SINGLETON(MemoryMonitor);

MemoryMonitor::MemoryMonitor(void)
    : QObject(0)
    , currentBytes_u64(0)
    , totalAlloc_u64(0)
    , totalFree_u64(0)
    , isInitial(true)
    , freeQueueSize_i(0)
    , freeQueue_ems(0)
    , freeQueue_timer(0)
{
}

void * MemoryMonitor::newMemory(void *ptr, const size_t bytes,
                                 void * parent,
                                 const QString & varName,
                                 const QString & typeName,
                                 const QString & funcInfo,
                                 const QString & fileName,
                                 const int fileLine)
{
    BFUNCTION(bytes, varName, typeName);

    if ( ! ptr)
    {
        BMEMORY("***Unable to allocate %1 bytes for %2 %3",
                bytes, typeName, varName);
        BMEMORY("   in %1 [%2:%3]", funcInfo, fileName, fileLine);
    }

    if (alloc_pv_item_map.contains(ptr))
    {
        MemoryMonitorItem mmi = alloc_pv_item_map.take(ptr);
        BWARNING("===Memory reallocated not free'd %1 %2 at %3",
                 mmi.getTypeName(),
                 mmi.getVarName(),
                 QDateTime::fromMSecsSinceEpoch(mmi.getAllocEms()).toString());
        BWARNING("   in %1 [%2:%3]",
                 mmi.getAllocFunction(),
                 mmi.getFileName(),
                 mmi.getFileLine());
        free(mmi.getPointer());
        mmi.setFreeFunction(funcInfo);
        mmi.setFreeEms(QDateTime::currentMSecsSinceEpoch());
        free_pv_item_mmap.insertMulti(mmi.getPointer(), mmi);
        free_item_q.append(mmi);
        totalFree_u64 += mmi.getBytes();
        currentBytes_u64 -= mmi.getBytes();
    }

    MemoryMonitorItem mmi;
    mmi.setPointer(ptr);
    mmi.setBytes(bytes);
    mmi.setParent(parent);
    mmi.setVarName(varName);
    mmi.setTypeName(typeName);
    mmi.setAllocFunction(funcInfo);
    mmi.setFileName(fileName);
    mmi.setFileLine(fileLine);
    mmi.setThread(QThread::currentThreadId());
    mmi.setAllocEms(QDateTime::currentMSecsSinceEpoch());
    if (isInitial)  mmi.setInitial();
    BTODO("isQObject()");
    BALLOC("---%1 bytes allocated at 0x%2 for %3 %4",
           bytes, QString::number(qptrdiff(ptr), 16), typeName, varName);
    BALLOC("   in %1 [%2:%3]",
             mmi.getAllocFunction(),
             mmi.getFileName(),
             mmi.getFileLine());
    alloc_pv_item_map.insert(ptr, mmi);
    totalAlloc_u64 += bytes;
    currentBytes_u64 += bytes;
    return ptr;
}

void MemoryMonitor::freeMemory(void * ptr,
                                  const QString & funcInfo)
{
    BFUNCTION(QString::number(qptrdiff(ptr), 16), funcInfo);
    if (alloc_pv_item_map.contains(ptr))
    {
        MemoryMonitorItem mmi = alloc_pv_item_map.take(ptr);
        BALLOC("---Freeing memory in %1 after %2ms", funcInfo,
               QDateTime::currentMSecsSinceEpoch() - mmi.getAllocEms());
        BALLOC("   %1 bytes allocated at 0x%2 for %3 %4",
               mmi.getBytes(), QString::number(qptrdiff(ptr), 16),
               mmi.getTypeName(), mmi.getVarName());
        BALLOC("   in %1 [%2:%3]",
                 mmi.getAllocFunction(),
                 mmi.getFileName(),
                 mmi.getFileLine());
        BEXPECTEQ(quint32(mmi.getThread()),
                  quint32(QThread::currentThreadId()));
        free(mmi.getPointer());
        mmi.setFreeFunction(funcInfo);
        mmi.setFreeEms(QDateTime::currentMSecsSinceEpoch());
        free_pv_item_mmap.insertMulti(mmi.getPointer(), mmi);
        free_item_q.append(mmi);
        totalFree_u64 += mmi.getBytes();
        currentBytes_u64 -= mmi.getBytes();
    }
    else
    {
        if (free_pv_item_mmap.contains(ptr))
        {
            MemoryMonitorItem mmi = free_pv_item_mmap.value(ptr);
            BWARNING("===Memory already free'd: %1 %2 at %3",
                     mmi.getTypeName(), mmi.getVarName(),
                     QDateTime::fromMSecsSinceEpoch(mmi.getFreeEms()).toString());
            BWARNING("   free'd in %1", mmi.getFreeFunction());
        }
        else
        {
            BWARNING("===Free'ing memory not allocated by MemoryMonitor at 0x%1",
                     QString::number(qptrdiff(ptr), 16));
            free(ptr);
        }
    }
    BTODO("check children free'd");
}

void MemoryMonitor::adopt(void * ptr,
                            void * newParent)
{
    if (alloc_pv_item_map.contains(ptr))
        alloc_pv_item_map[ptr].setParent(newParent);
}

void MemoryMonitor::initialPoint(void)
{
    BMUSTDO(Q_FUNC_INFO);
}

void MemoryMonitor::freeQueueCheck(void)
{
    BMUSTDO(Q_FUNC_INFO);
}

void MemoryMonitor::destroyingObject(void)
{
    BMUSTDO(Q_FUNC_INFO);
}

void MemoryMonitor::blogReport(const bool detailed)
{
    BLOGMSG("---MemoryMonitor report:");
    BLOGMSG("   Current Bytes allocated:    %1", currentBytes_u64);
    BLOGMSG("   Total Bytes allocated:      %1", totalAlloc_u64);
    BLOGMSG("   Total Bytes freed:          %1 (%2)", totalFree_u64,
            totalAlloc_u64 - totalFree_u64 - currentBytes_u64);
    BLOGMSG("   Current Memory allocations: %1",
            alloc_pv_item_map.size());

    if (detailed)
    {
        foreach(MemoryMonitorItem mmi, alloc_pv_item_map.values())
        {
            BLOGMSG("   %1 bytes at 0x%2 %3 %4",
                    mmi.getBytes(),
                    QString::number(qptrdiff(mmi.getPointer()), 16),
                    mmi.getTypeName(),
                    mmi.getVarName());
            BLOGMSG("  _[%1:%2] for %3ms ago %4",
                    mmi.getFileName(),
                    mmi.getFileLine(),
                    QDateTime::currentMSecsSinceEpoch() - mmi.getAllocEms(),
                    mmi.getAllocFunction());
        }
    }
}
