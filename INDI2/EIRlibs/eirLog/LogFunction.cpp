#include "LogFunction.h"

#include <QThread>
#include <QVariant>

#include <eirBase/Severity.h>

#include "Log.h"
#include "LogItem.h"

QList<QThread *> LogFunction::depth_thread_list;

LogFunction::LogFunction(bool slot,
                         const QString & moduleName,
                         const QString & fileName,
                         const int fileLine,
                         const QString & prettyFunction,
                         const QVariant & v1,
                         const QVariant & v2,
                         const QVariant & v3,
                         const QVariant & v4,
                         const QVariant & v5,
                         const QVariant & v6,
                         const QVariant & v7,
                         const QVariant & v8,
                         const QVariant & v9)
    : fileName_s(fileName)
    , fileLine_i(fileLine)
    , fullFunction_s(prettyFunction)
{
    QVariantList varList;
    varList << v1 << v2 << v3 << v4 << v5 << v6 << v7 << v8 << v9;

    _et.start();
    splitFuncInfo(fullFunction_s);
    depth_thread_list.append(QThread::currentThread());
    int depth = depth_thread_list.count(QThread::currentThread());

    LogItem li;
    li.setSeverity(slot ? Severity::Signal : Severity::Enter);
    li.setModuleName(moduleName);
    li.setFileName(fileName_s);
    li.setFileLine(fileLine_i);
    li.setClassName(className_s);
    li.setFunctionName(functionName_s);
    li.setDepth(depth);
    li.setFormat("Entering %2::%1()");
    QVariantList vl;
    vl << functionName_s << className_s;
    li.setVariables(vl);
    LOG(add(li));

    int k = 0;
    li.setSeverity(Severity::EnterArg);
    li.setFormat("%1 = {%2}");
    foreach (QVariant var, varList)
    {
        QVariantList vl;
        vl << QString((functionArgs_qsl.size() > k)
                      ? functionArgs_qsl.at(k++)
                      : "???");
        vl << var;
        li.setVariables(vl);
        LOG(add(li));
    }
}

LogFunction::LogFunction(const QString & pretty)
    : fileLine_i(0)
    , fullFunction_s(pretty)
{
    splitFuncInfo(pretty);
} // pretty c'tor

LogFunction::~LogFunction()
{
    if (fileLine_i)
    {
        LogItem li;
        li.setSeverity(Severity::Leave);
        li.setFileName(fileName_s);
        li.setFileLine(fileLine_i);
        li.setFunctionName(fullFunction_s);
        li.setDepth(depth_thread_list.count(QThread::currentThread()));
        li.setFormat(qPrintable("Leaving  %3::%1() after %2ms"
               + QString(return_var.isNull() ? "" : " = {%4}")));
        QVariantList vl;
        vl << functionName_s << _et.elapsed() << className_s;
        if ( ! return_var.isNull()) vl << return_var;
        li.setVariables(vl);
        LOG(add(li));
        depth_thread_list.removeOne(QThread::currentThread());
    }
} // d'tor

void LogFunction::setReturn(const QVariant & rv)
{
    return_var = rv;
}

QString LogFunction::className(void) const
{
    return className_s;
}

QString LogFunction::functionName(void) const
{
    return functionName_s;
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
        templateInfo_s = fn.mid(xLeftBrack, xRightBrack-xLeftBrack-1);
    postFunction_s = fn.mid(xRightParen+1).simplified();
    QString args = fn.mid(xLeftParen+1, xRightParen - xLeftParen - 1);
    if (args.isEmpty())
        functionArgs_qsl.clear();
    else
        functionArgs_qsl = args.split(", ");
    int xPreName = fn.lastIndexOf(" ", xLeftParen);
    QStringList names = fn.mid(xPreName+1, xLeftParen - xPreName - 1).split("::");
    if (names.size() > 1)
        className_s = names.at(0), functionName_s = names.at(1);
    else
        className_s.clear(), functionName_s = names.at(0);
    preFunction_s = fn.left(xPreName);
} // splitFuncInfo()
