#ifndef LOGFILTER_H
#define LOGFILTER_H
#include "eirExe.h"

#include "../eirCore/VariableSet.h"

#include <QtCore/QString>

#include "../eirBase/Severity.h"
#include "../eirBase/SeverityMask.h"

class LogItem;

#define LOGFILTER_VARIABLESET(TIVD) \
    TIVD(QString, Module,   QString, QString()) \
    TIVD(QString, Class,    QString, QString()) \
    TIVD(QString, Function, QString, QString()) \
    TIVD(QString, FileName, QString, QString()) \
    TIVD(QString, FileLines,QString, QString()) \
    TIVD(QString, Report,   QString, QString()) \
    TIVD(QString, Ignore,   QString, QString()) \

class EIREXESHARED_EXPORT LogFilter : public VariableSet
{
public:
    DECLARE_VARIABLESET(LOGFILTER_VARIABLESET);
    LogFilter(const QString & name=QString());
    LogFilter(const VariableSet & other);
    void calculate(void);
    int evaluate(const LogItem & li);

private:
    enum Checks
    {
        Module      = 0x01,
        Class       = 0x02,
        Function    = 0x03,
        File        = 0x04,
        Lines       = 0x05,
    };

private:
    int checkMask_i;
    SeverityMask report_mask;
    SeverityMask ignore_mask;
    int minLine_i;
    int maxLine_i;
};

#endif // LOGFILTER_H
