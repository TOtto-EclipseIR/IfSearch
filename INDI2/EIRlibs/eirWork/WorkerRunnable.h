#ifndef WORKERRUNNABLE_H
#define WORKERRUNNABLE_H

#include <QRunnable>

#include "AbstractWorker.h"
#include "WorkerObject.h"

class WorkerRunnable : public QRunnable
{
public:
    WorkerRunnable(WorkerObject * workerObject);
    virtual void run(void);

private:
    WorkerObject * worker_object;
};

#endif // WORKERRUNNABLE_H
