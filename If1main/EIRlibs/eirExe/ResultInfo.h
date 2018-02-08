#ifndef ResultInfo_H
#define ResultInfo_H
#include "eirExe.h"

#include <QSharedDataPointer>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "ResultCode.h"

class ResultInfoData;
class ResultId;
class Severity;

class EIREXESHARED_EXPORT ResultInfo
{
public:
    ResultInfo();
    ResultInfo(const ResultInfo &);
    ResultInfo(const ResultCode rc,
               const Severity sev,
               const ResultId & id,
               const QString & fmt,
               const QString & desc=QString(),
               const QStringList & names=QStringList());
    ResultInfo & operator=(const ResultInfo &);
    ~ResultInfo();

    ResultCode code(void) const;
    Severity severity(void) const;
    ResultId id(void) const;
    QString format(void) const;
    QString description(void) const;
    QStringList valueNames(void) const;
    bool isNull(void) const;
    
private:
    QSharedDataPointer<ResultInfoData> data;
};

#endif // ResultInfo_H
