#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "eirCfg.h"

#include <QObject>

#include <eirVar/VariableMap.h>

class ConfigurationSettings;

class EIRCFGSHARED_EXPORT Configuration : public QObject, VariableMap
{
    Q_OBJECT
public:
    explicit Configuration(QObject * parent=0);
    ConfigurationSettings * settings(const QString & name);

signals:

public slots:

};

#endif // CONFIGURATION_H
