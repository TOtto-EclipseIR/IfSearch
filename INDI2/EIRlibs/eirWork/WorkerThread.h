#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

#include "AbstractWorker.h"
#include "WorkerObject.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(WorkerObject * workerObject,
                 QObject *parent = 0);
    virtual void run(void);

private:
    WorkerObject * worker_object;

signals:

public slots:

};

#endif // WORKERTHREAD_H
