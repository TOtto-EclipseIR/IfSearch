#ifndef RESULT_H
#define RESULT_H
#include "eirExe.h"

#include <QtCore/QVariantList>

#include "../eirTypes/MillisecondTime.h"

#include "ResultCode.h"

class Severity;
class ResultInfo;
class ResultId;

class EIREXESHARED_EXPORT Result
{
public:
    Result(void);
    Result(const ResultCode & rc,
           const QVariantList & varList=QVariantList());
    Result(const ResultCode & rc,
           const QVariant & v1,
           const QVariant & v2=QVariant(),
           const QVariant & v3=QVariant(),
           const QVariant & v4=QVariant());
    Result(const ResultId & id,
           const QVariantList & varList=QVariantList());
    Result(const ResultId & id,
           const QVariant & v1,
           const QVariant & v2=QVariant(),
           const QVariant & v3=QVariant(),
           const QVariant & v4=QVariant());
    Result(QVariantList &qvl);
    QVariantList toVariantList(void) const;
    bool isNull(void) const;
    ResultCode code(void) const;
    ResultInfo info(void) const;
    Severity severity(void) const;
    QString format(void) const;
    QString description(void) const;
    MillisecondTime time(void) const;
    QVariantList vars(void) const;
    QVariant value(const QString & name);
    QString toString(void) const;

private:
    ResultCode _rc;
    MillisecondTime _mst;
    QVariantList var_list;
};
Q_DECLARE_METATYPE(Result)

#endif // RESULT_H
