#ifndef LOGGER_H
#define LOGGER_H
#include "eirLog.h"

class QUrl;

#include "BasicLogOutput.h"

class EIRLOGSHARED_EXPORT Logger
{
    friend class eirLog;

public:
    static Logger * instance(void);

public:
    void add(LogItem li);
    bool expect(const QString & mod,
                const QString & file,
                const int ln,
                const QString & fn,
                const bool expnot,
                const bool expr,
                const QString & text,
                const QString & text2=QString());
    bool addDevice(const QString & name,
                   const QUrl & url);

private:
    Logger(void);
    static Logger * instance_p;

private:
    BasicLogOutput _blo;
};

#endif // LOGGER_H
