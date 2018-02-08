#include "WorkerThread.h"

WorkerThread::WorkerThread(WorkerObject * workerObject,
                           QObject * parent)
    : QThread(parent)
    , worker_object(workerObject)
{
}

void WorkerThread::run(void)
{
    worker_object->process();
}
