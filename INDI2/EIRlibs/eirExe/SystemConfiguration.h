#ifndef SYSTEMCONFIGURATION_H
#define SYSTEMCONFIGURATION_H
#include "eirExe.h"

#include <eirCfg/ConfigurationStack.h>

class EIREXESHARED_EXPORT SystemConfiguration : public ConfigurationStack
{
    Q_OBJECT
public:
    explicit SystemConfiguration(QObject *parent=0);

signals:

public slots:

};

#endif // SYSTEMCONFIGURATION_H
