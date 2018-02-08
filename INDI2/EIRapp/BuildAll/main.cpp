#include <QCoreApplication>

#include <QDebug>
#include <QStringList>
#include <QTimer>

#include <eirBase/eirBase.h>
#include <eirType/eirType.h>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0) : QObject(parent) {}

public slots:
    void run()
    {
        // Do processing here
        qDebug() << eirType::instance.name();
        foreach (QString moduleName, Module::moduleNames())
            qDebug() << moduleName
                     << Module::version(moduleName).toString();

        emit finished();
    }

signals:
    void finished();
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
