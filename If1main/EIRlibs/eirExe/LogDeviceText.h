#ifndef LOGDEVICETEXT_H
#define LOGDEVICETEXT_H
#include "eirExe.h"

#include "LogDevice.h"

class EIREXESHARED_EXPORT LogDeviceText : public LogDevice
{
    Q_OBJECT
public:
    LogDeviceText(const VariableSet & config,
                  Logger * parent);
    ~LogDeviceText();
    bool isValid(void) const;
    void write(const Severity severity,
               const QString & string);
    void flush(void);

private:
    QFile _file;
    QTextStream _stream;
};


#endif // LOGDEVICETEXT_H
