#ifndef LOGDEVICE_H
#define LOGDEVICE_H

#include <QtCore/QObject>
#include <QtCore/QMap>

/*! @file LogDevice.h LogDevice.h contains the base log output device class
  *
  * @todo LogDeviceMailto
  *         void setSendSeverity(const Severity sev);
  *         void setMaxItems(const int maxItems);
  *         void send(void);
  *         void setUrl(const QUrl & url);
  *
  */


#include "LogItem.h"
class Logger;
class LogFilter;

#include "../eirBase/Enumeration.h"
#define LOGDEVICETYPE_ENUM(NV) \
    NV(Null, = 0)   \
    NV(StdIo,)      \
    NV(TxtFile,)    \
    NV(Widget,)     \
    NV(EblFile,)    \
    NV(System,)     \
    NV(Sql,)        \
    NV(Settings,)   \
    NV(Invalid,)    \

class LogDeviceType : public Enumeration
{
    DECLARE_ENUMERATION(LogDeviceType, LOGDEVICETYPE_ENUM)
};

class EIREXESHARED_EXPORT LogDevice : public QObject
{
    Q_OBJECT
public:
    ~LogDevice();
    static LogDevice * create(const VariableSet & config,
                              Logger * parent);
    virtual bool isValid(void) const;
    virtual bool isTroll(void) const;
    quint32 options(void) const;

protected:
    virtual bool alreadyExists(void);
    virtual void write(const Severity severity,
                       const QString & string) = 0;

private:
    void loadFilters(const VariableSet & filterConfig);
    bool evaluate(const LogItem & li);

signals:

public slots:
    void write(LogItem li);
    virtual void start(void);
    virtual void finish(void);
    virtual void rollover(void);
    virtual void flush(void);
    virtual void remove(qint64 beforeEms);
    virtual void thin(qint64 beforeEms, int sev);

protected:
    LogDevice(const VariableSet & config, Logger * parent);


private:
    Logger * _logger;
    VariableSet config_vars;
    LogDeviceType _type;
    quint32 option_flags;
    QMap<QString, LogFilter> name_filter_map;
};

#endif // LOGDEVICE_H
