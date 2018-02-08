#ifndef LOGGER_H
#define LOGGER_H
#include "eirExe.h"

#include <QtCore/QObject>

#include <QtCore/QPair>
#include <QtCore/QQueue>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVariant>

#include "../eirBase/Severity.h"

class Result;
class ResultSet;
class VariableSet;
class VariableSettingsList;
class LogDevice;
class LogItem;

class EIREXESHARED_EXPORT Logger : public QObject
{
    Q_OBJECT

public:
    static enum States
    {
        Initial,
        Instance,
        Ready,
        Running,
        // Paused,
        Finished,
    } state;

    typedef QPair<LogDevice *, LogItem> DeviceItem;

public:
    static Logger * instance(void);
    static void create(QObject * parent);
    ~Logger();

    void add(const Severity sev,
             const QString & mod,
             const QString & file,
             const int ln,
             const QString & fn,
             const QString & fmt,
             const QVariant & v1=QVariant(),
             const QVariant & v2=QVariant(),
             const QVariant & v3=QVariant(),
             const QVariant & v4=QVariant());
    void add(const Severity sev,
             const QString & mod,
             const QString & file,
             const int ln,
             const QString & fn,
             const QString & fmt,
             const QVariantList & vl);
    void add(LogItem & li);
    void add(const Result & r);
    void add(const ResultSet & rl);
    void dump(const VariableSet & vset) const;
    void dump(const VariableSettingsList & vsl) const;
    void dumpHex(const QString & mod,
                 const QString & file,
                 const int ln,
                 const QString & fn,
                 const QString & name,
                 void const * const ptr,
                 const unsigned size);
    bool expect(const QString & mod,
                const QString & file,
                const int ln,
                const QString & fn,
                const bool expnot,
                const bool expr,
                const QString & text,
                const QString & text2=QString());
    void configure(const VariableSet & config);
    void removeDevice(const QString & name);
    void addDevice(const QString & name,
                   const VariableSet & config);
    bool hasStdout(void) const;
    bool hasStderr(void) const;
    QList<LogDevice *> devices(void) const;
    QStringList deviceNames(void) const;
    void start(void);
    void finish(void);

signals:

public slots:
    void flush(void);

private slots:
    void pump(void);

private:
    Logger(QObject * parent);
    QString threadName(QThread * thread);
    void enqueue(const LogItem & li);

private:
    static Logger * singleton;
    QThread * app_thread;
    QQueue<DeviceItem> write_queue;
    QReadWriteLock write_queue_lock;
    QMap<QThread *, QString> thread_name_map;
    QMap<QString, LogDevice *> name_device_map;
};

#endif // LOGGER_H
