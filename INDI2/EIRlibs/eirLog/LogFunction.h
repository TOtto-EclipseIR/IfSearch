#ifndef LOGFUNCTION_H
#define LOGFUNCTION_H
#include "eirLog.h"

#include <QElapsedTimer>
#include <QString>
#include <QStringList>
#include <QVariant>
class QThread;

class EIRLOGSHARED_EXPORT LogFunction
{
public:
    LogFunction(bool slot,
                const QString & moduleName,
                const QString & fileName,
                const int fileLine,
                const QString & prettyFunction,
                const QVariant & v1=QVariant(),
                const QVariant & v2=QVariant(),
                const QVariant & v3=QVariant(),
                const QVariant & v4=QVariant(),
                const QVariant & v5=QVariant(),
                const QVariant & v6=QVariant(),
                const QVariant & v7=QVariant(),
                const QVariant & v8=QVariant(),
                const QVariant & v9=QVariant());
    LogFunction(const QString & pretty);
    ~LogFunction();
    QString className(void) const;
    QString functionName(void) const;
    void setReturn(const QVariant & rv);
    qint64 msecs(void) const;

private:
    void splitFuncInfo(const QString & fn);

private:
    QString preFunction_s;
    QString className_s;
    QString functionName_s;
    QString postFunction_s;
    QStringList functionArgs_qsl;
    QString templateInfo_s;
    QString fileName_s;
    int fileLine_i;
    QString fullFunction_s;
    QVariant return_var;
    QElapsedTimer _et;
    static QList<QThread *> depth_thread_list;
};

#endif // LOGFUNCTION_H
