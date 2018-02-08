#include "Console.h"

#include <QtCore/QSettings>

#include "Log.h"

Console::Console(int argc, char ** argv)
    : QCoreApplication(argc, argv)
    , ExeSupport((QCoreApplication *)this)
{
}

void Console::initializeLogger(const Severity stdoutSeverity,
                               const Severity stderrSeverity)
{
    FUNCTION();
    ExeSupport::initializeLogger();

    OBJPOINTER(Logger, Logger::instance());
    if ( ! Logger::instance()->hasStderr()
            && ! stderrSeverity.isNull())
    {
        TODO("Add stderr device");
    }
    if ( ! Logger::instance()->hasStdout()
            && ! stdoutSeverity.isNull())
    {
        TODO("Add stdout device");
    }

}


Console::~Console()
{
    if (Logger::instance())    Logger::instance()->flush();
}
