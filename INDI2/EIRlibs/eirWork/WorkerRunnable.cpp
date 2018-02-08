#include "WorkerRunnable.h"

WorkerRunnable::WorkerRunnable(WorkerObject * workerObject)
    : worker_object(workerObject)
{
}

void WorkerRunnable::run(void)
{
    worker_object->process();
}
