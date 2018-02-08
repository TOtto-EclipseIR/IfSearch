#ifndef BASICLOGOUTPUT_H
#define BASICLOGOUTPUT_H

class QFile;
class QUrl;

#include "LogFilter.h"
#include "LogFilterSet.h"
class LogItem;

class BasicLogOutput
{
    enum OutputType
    {
        LogFile = 0,
        TraceFile,
        StdOut,
        StdErr,
        nFiles,
        BaseLog,
        nMaximum,
    };

public:
    explicit BasicLogOutput(void);
    QString sysEnvVarName(const enum OutputType otype) const;
    bool set(const enum OutputType otype,
             const QUrl & url);
    void write(LogItem &li);
    void flush(void);

private:
    LogFilterSet _filters[nMaximum];
    QFile * _file[nFiles];
    int optionMask_i[nMaximum];
};

#endif // BASICLOGOUTPUT_H
