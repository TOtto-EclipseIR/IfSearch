#include "Logger.h"

#include "LogItem.h"

Logger * Logger::instance_p = 0;

Logger::Logger(void)
{
}

Logger * Logger::instance(void)
{
    return instance_p;
}

void Logger::add(LogItem li)
{
    _blo.write(li);
}

bool Logger::expect(const QString & mod,
                    const QString & file,
                    const int ln,
                    const QString & fn,
                    const bool expnot,
                    const bool expr,
                    const QString & text,
                    const QString & text2)
{
    if (expnot == expr)
        add(LogItem(Severity::Expect, mod, file, ln, fn,
            text2.isNull()
                ? QString("Expectation " + QString(expnot ? "false" : "true") +" Failed: %1")
                : QString("Expected %1 " + QString(expnot ? "!=" : "==") + QString("%2")),
            text, text2));
    return expr;
}

