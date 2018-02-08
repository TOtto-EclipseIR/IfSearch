#include "LogDeviceNull.h"

LogDeviceNull::LogDeviceNull(const VariableSet & config,
                             Logger * parent)
    : LogDevice(config, parent)
{
}

LogDeviceNull::~LogDeviceNull()
{
}

bool LogDeviceNull::isValid(void) const
{
    return true;
}

void LogDeviceNull::write(const Severity severity,
                          const QString & string)
{
    (void)severity, (void)string;
}
