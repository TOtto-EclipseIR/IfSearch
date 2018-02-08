#ifndef CONSOLE_H
#define CONSOLE_H
#include "eirExe.h"

#include <QtCore/QCoreApplication>

#include "../eirBase/Severity.h"

#include "ExeSupport.h"

class VariableSettingsList;

class EIREXESHARED_EXPORT Console : public QCoreApplication
                                  , public ExeSupport
{
    Q_OBJECT

public:
    explicit Console(int argc, char ** argv);
    ~Console();
    void setVersion(const VersionInfo & vi);
    void initializeLogger(const Severity stdoutSeverity=Severity::Info,
                          const Severity stderrSeverity=Severity::Stern);

signals:

public slots:

private:

};

#endif // CONSOLE_H
