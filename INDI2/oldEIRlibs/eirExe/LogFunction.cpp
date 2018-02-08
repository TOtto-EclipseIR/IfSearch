#include "LogFunction.h"

#include <QThread>
#include <QVariant>

#include "../eirBase/Severity.h"
#include "Log.h"
#include "LogItem.h"

QList<QThread *> LogFunction::depth_threadList;

LogFunction::LogFunction(const bool slot,
                         const QString & file,
                         const int line,
                         const QString & fn,
                         const QVariant & v1,
                         const QVariant & v2,
                         const QVariant & v3,
                         const QVariant & v4)
    : fi_s(file)
    , ln_i(line)
    , fullFn_s(fn)
{
    QVariantList vl;
    if ( ! v4.isNull())
        vl << v1 << v2 << v3 << v4;
    else if ( ! v3.isNull())
        vl << v1 << v2 << v3;
    else if ( ! v2.isNull())
        vl << v1 << v2;
    else if ( ! v1.isNull())
        vl << v1;
    ctor(slot, vl);
}

LogFunction::LogFunction(const bool slot,
                         const QString & file,
                         const int line,
                         const QString & fn,
                         const QVariantList & vl)
    : fi_s(file)
    , ln_i(line)
    , fullFn_s(fn)
{
    ctor(slot, vl);
}

void LogFunction::ctor(const bool slot, const QVariantList varList)
{
    _et.start();
    splitFuncInfo(fullFn_s);
    depth_threadList.append(QThread::currentThread());
    int depth = depth_threadList.count(QThread::currentThread());

    LogItem li;
    li.setSeverity(slot ? Severity::Signal : Severity::Enter);
    li.setFileName(fi_s);
    li.setFileLine(ln_i);
    li.setFunction(fullFn_s);
    li.setDepth(depth);
    li.set("Entering %2::%1()");
    QVariantList vl;
    vl << fnName_s << clsName_s;
    li.set(vl);
    LOG(add(li));

    int k = 0;
    li.setSeverity(Severity::EnterArg);
    li.set("%1 = {%2}");
    foreach (QVariant var, varList)
    {
        QVariantList vl;
        vl << QString((fnArgs_qsl.size() > k)
                      ? fnArgs_qsl.at(k++)
                      : "???");
        vl << var;
        li.set(vl);
        LOG(add(li));
    }
} // ctor()

LogFunction::LogFunction(const QString & pretty)
    : ln_i(0)
    , fullFn_s(pretty)
{
    splitFuncInfo(pretty);
} // pretty c'tor

LogFunction::~LogFunction()
{
    if (ln_i)
    {
        LogItem li;
        li.setSeverity(Severity::Leave);
        li.setFileName(fi_s);
        li.setFileLine(ln_i);
        li.setFunction(fullFn_s);
        li.setDepth(depth_threadList.count(QThread::currentThread()));
        li.set(qPrintable("Leaving  %3::%1() after %2ms"
               + QString(return_var.isNull() ? "" : " = {%4}")));
        QVariantList vl;
        vl << fnName_s << _et.elapsed() << clsName_s;
        if ( ! return_var.isNull()) vl << return_var;
        li.set(vl);
        LOG(add(li));
        depth_threadList.removeOne(QThread::currentThread());
    }
} // d'tor

void LogFunction::setReturn(const QVariant & rv)
{
    return_var = rv;
}

QString LogFunction::className(void) const
{
    return clsName_s;
}

QString LogFunction::functionName(void) const
{
    return fnName_s;
}

qint64 LogFunction::msecs(void) const
{
    return _et.elapsed();
}

void LogFunction::splitFuncInfo(const QString & fn)
{
    int xRightBrack = fn.lastIndexOf(']');
    int xLeftBrack  = fn.lastIndexOf('[', xRightBrack);
    int xRightParen = fn.lastIndexOf(")");
    int xLeftParen  = fn.lastIndexOf("(", xRightParen);
    if (xRightBrack > xLeftBrack)
        tplInfo_s = fn.mid(xLeftBrack, xRightBrack-xLeftBrack-1);
    postFn_s = fn.mid(xRightParen+1).simplified();
    QString args = fn.mid(xLeftParen+1, xRightParen - xLeftParen - 1);
    if (args.isEmpty())
        fnArgs_qsl.clear();
    else
        fnArgs_qsl = args.split(", ");
    int xPreName = fn.lastIndexOf(" ", xLeftParen);
    QStringList names = fn.mid(xPreName+1, xLeftParen - xPreName - 1).split("::");
    if (names.size() > 1)
        clsName_s = names.at(0), fnName_s = names.at(1);
    else
        clsName_s.clear(), fnName_s = names.at(0);
    preFn_s = fn.left(xPreName);
} // splitFuncInfo()
