/*! @file MemoryMonitor.h Declarations for MemoryMonitor class
*
*/
#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H
#include "eirBase.h"

#include <QObject>

#include <QMap>
#include <QMultiMap>
#include <QQueue>
#include <QTimer>

#include "Singleton.h"

#include "MemoryMonitorItem.h"

class EIRBASESHARED_EXPORT MemoryMonitor : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(MemoryMonitor);

public:
    typedef qint64 checkpoint_t;

private:
    struct MemoryCheckpointItem
    {
        checkpoint_t _ckpt;
        Qt::HANDLE _thread;
        quint64 thread_bytes;
    };

public:
    void setFreeQueueMsec(const qint64 ems);
    void setFreeQueueTime(const qint64 ems);
    void setFreeQueueSize(const int items);

    void * newMemory(void * ptr,
                     const size_t bytes,
                     void * parent,
                     const QString & varName,
                     const QString & typeName,
                     const QString & funcInfo,
                     const QString & fileName,
                     const int fileLine);
    void freeMemory(void * ptr,
              const QString & funcInfo);
    void adopt(void * ptr,
               void * newParent);

    quint64 currentBytes(void) const;
    quint64 totalAlloc(void) const;
    quint64 totalFree(void) const;
    int allocSize(void) const;

    checkpoint_t makeCheckpoint(void * parent);
    bool checkCheckpoint(const checkpoint_t cp);
    bool killCheckpoint(const checkpoint_t cp);

    void blogReport(const bool detailed=false);

signals:

public slots:
    void initialPoint(void);

private slots:
    void freeQueueCheck(void);
    void destroyingObject(void);

private:
    QMap<void *, MemoryMonitorItem> alloc_pv_item_map;
    QMultiMap<void *, MemoryMonitorItem> free_pv_item_mmap;
    QQueue<MemoryMonitorItem> free_item_q;
    QMap<checkpoint_t, MemoryCheckpointItem> ckpt_item_map;
    quint64 currentBytes_u64;
    quint64 totalAlloc_u64;
    quint64 totalFree_u64;
    bool isInitial;
    int freeQueueSize_i;
    qint64 freeQueue_ems;
    QTimer * freeQueue_timer;
};

#endif // MEMORYMONITOR_H
