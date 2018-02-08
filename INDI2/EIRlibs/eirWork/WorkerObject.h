#ifndef WORKEROBJECT_H
#define WORKEROBJECT_H

#include <QObject>
#include <QTimer>

#include "AbstractWorker.h"

class WorkerObject : public QObject
{
    Q_OBJECT
public:
    WorkerObject(AbstractWorker * worker,
                 QObject * parent=0);
    WorkerObject(AbstractWorker * worker,
                 int progressMsec,
                 QObject * parent=0);
    ~WorkerObject();

signals:
    void progress(int min, int current, int max);
    void error(Results results);
    void warning(Results results);
    void complete(Results results);

public slots:
    void process(void);

private slots:
    void getProgress(void);

private:
    AbstractWorker * _worker;
    QTimer * progress_timer;

};

#endif // WORKEROBJECT_H
