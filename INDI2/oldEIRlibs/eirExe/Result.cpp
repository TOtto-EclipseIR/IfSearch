#include "Result.h"

#include "../eirBase/Severity.h"
#include "../eirCore/eirCore.h"
#include "../eirExe/Log.h"

#include "ResultInfo.h"
#include "ResultId.h"
#include "ResultCode.h"
#include "ResultRegistry.h"

Result::Result(void)
{
}

Result::Result(const ResultCode &rc,
               const QVariantList & varList)
    : _rc(rc)
    , _mst(MillisecondTime::current())
    , var_list(varList)
{
    EXPECTNE(0, _rc);
}

Result::Result(const ResultCode &rc,
               const QVariant & v1,
               const QVariant & v2,
               const QVariant & v3,
               const QVariant & v4)
    : _rc(rc)
    , _mst(MillisecondTime::current())
    , var_list(QVariantList() << v1)
{
    EXPECTNE(0, _rc);
    if (v2.isValid())   var_list << v2;
    if (v3.isValid())   var_list << v3;
    if (v4.isValid())   var_list << v4;
}

Result::Result(const ResultId & id,
               const QVariantList & varList)
    : _rc(ResultRegistry::code(id))
    , _mst(MillisecondTime::current())
    , var_list(varList)
{
    EXPECTNOT(_rc.isNull());
}

Result::Result(const ResultId & id,
               const QVariant & v1,
               const QVariant & v2,
               const QVariant & v3,
               const QVariant & v4)
    : _rc(ResultRegistry::code(id))
    , _mst(MillisecondTime::current())
    , var_list(QVariantList() << v1)
{
    EXPECTNOT(_rc.isNull());
    if (v2.isValid())   var_list << v2;
    if (v3.isValid())   var_list << v3;
    if (v4.isValid())   var_list << v4;
}

Result::Result(QVariantList & qvl)
    : _rc(qvl.takeFirst().toULongLong())
    , _mst(qvl.takeFirst().toLongLong())
    , var_list(qvl)
{
    EXPECTNE(0, _rc);
}

QVariantList Result::toVariantList(void) const
{
    QVariantList qvl;
    qvl << quint64(_rc);
    qvl << qint64(_mst);
    qvl << var_list;
    return qvl;
}

bool Result::isNull(void) const
{
    return _rc.isNull();
}

ResultCode Result::code(void) const
{
    return _rc;
}

Severity Result::severity(void) const
{
    return ResultRegistry::info(_rc).severity();
}

QString Result::format(void) const
{
    return ResultRegistry::info(_rc).format();
}

QString Result::description(void) const
{
    return ResultRegistry::info(_rc).description();
}

MillisecondTime Result::time(void) const
{
    return _mst;
}

QVariantList Result::vars(void) const
{
    return var_list;
}

QVariant Result::value(const QString & name)
{
    ResultInfo ri = ResultRegistry::info(_rc);
    if (ri.isNull())  return QVariant();
    int x = ri.valueNames().indexOf(name);
    if (x < 0) return QVariant();
    return var_list.at(x);
}

QString Result::toString(void) const
{
    QString result = _mst.toString("hh:mm:ss.zzz ");
    return result + eirBase::formatMessage(format(), var_list);
}
