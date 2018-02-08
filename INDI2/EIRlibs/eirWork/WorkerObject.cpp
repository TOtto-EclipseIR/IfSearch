#include "WorkerObject.h"

WorkerObject::WorkerObject(AbstractWorker * worker,
                           QObject * parent)
    : QObject(parent)
    , _worker(worker)
    , progress_timer(0)
{
}

WorkerObject::WorkerObject(AbstractWorker * worker,
                           int progressMsec,
                           QObject * parent)
    : QObject(parent)
    , _worker(worker)
    , progress_timer(new QTimer(this))
{
    progress_timer->setInterval(progressMsec);
    progress_timer->setSingleShot(false);
}

WorkerObject::~WorkerObject()
{
    if (progress_timer) delete progress_timer;
}

void WorkerObject::process(void)
{
    if (progress_timer) progress_timer->start();
    Severity sev = _worker->process();
    /* busy working */
    Results res; // = _worker->Results();
    if (progress_timer) progress_timer->stop();
    getProgress();
    if (sev.isError())
        emit error(res);
    else if (sev.isWarning())
        emit warning(res);
    else
        emit complete(res);
}

void WorkerObject::getProgress(void)
{
    int min, current, max;
    _worker->getProgress(&min, &current, &max);
    emit progress(min, current, max);
}
