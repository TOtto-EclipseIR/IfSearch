#ifndef LOGDEVICENULL_H
#define LOGDEVICENULL_H
#include "eirExe.h"

#include "LogDevice.h"

class EIREXESHARED_EXPORT LogDeviceNull : public LogDevice
{
    Q_OBJECT
public:
    LogDeviceNull(const VariableSet & config,
                  Logger * parent);
    ~LogDeviceNull();
    bool isValid(void) const;
    void write(const Severity severity,
               const QString & string);
};

#endif // LOGDEVICENULL_H
