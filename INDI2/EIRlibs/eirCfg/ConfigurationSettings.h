#ifndef CONFIGURATIONSETTINGS_H
#define CONFIGURATIONSETTINGS_H
#include "eirCfg.h"

#include <QObject>
#include <QSettings>

class Configuration;

class EIRCFGSHARED_EXPORT ConfigurationSettings : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationSettings(const QString & name,
                                   Configuration * config);

public:
    static bool exists(const QString & name);

signals:

public slots:

private:
    static QSettings * fileSettings(const QString & fileName);
    static QSettings * appSettings(const QString & orgName,
                                   const QString & appName);

private:
    Configuration * _config;
    QSettings * _settings;
};

#endif // CONFIGURATIONSETTINGS_H
