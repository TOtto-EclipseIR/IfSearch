#ifndef LOGITEM_H
#define LOGITEM_H
#include "eirExe.h"

#include <eirBase/DataProperty.h>

#include "../eirCore/VariableSet.h"

#include "../eirBase/Severity.h"
#include "../eirTypes/MillisecondTime.h"

#define LOGITEM_VARIABLESET(TIVD) \
    TIVD(MillisecondTime, Time, qint64, 0) \
    TIVD(Severity, Severity, int, 0) \
    TIVD(QString, Module, QString, QString()) \
    TIVD(QString, FileName, QString, QString()) \
    TIVD(int, FileLine, int, 0) \
    TIVD(QString, Function, QString, QString()) \
    TIVD(QString, Thread, QString, QString()) \
    TIVD(int, Depth, int, 0) \

class EIREXESHARED_EXPORT LogItem : public VariableSet
{
public:
    DECLARE_VARIABLESET(LOGITEM_VARIABLESET);
    LogItem(const QString & name=QString("LogItem"));
    QString toString(quint32 options) const;
};

#endif // LOGITEM_H
