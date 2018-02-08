#ifndef LOGFUNCTION_H
#define LOGFUNCTION_H
#include "eirExe.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
class QThread;

class EIREXESHARED_EXPORT LogFunction
{
public:
    LogFunction(bool slot,
                const QString & file,
                const int line,
                const QString & fn,
                const QVariant & v1=QVariant(),
                const QVariant & v2=QVariant(),
                const QVariant & v3=QVariant(),
                const QVariant & v4=QVariant());
    LogFunction(bool slot,
                const QString & file,
                const int line,
                const QString & fn,
                const QVariantList & vl);
    LogFunction(const QString & pretty);
    ~LogFunction();
    void ctor(const bool slot,
              const QVariantList varList);
    QString className(void) const;
    QString functionName(void) const;
    void setReturn(const QVariant & rv);
    qint64 msecs(void) const;

private:
    void splitFuncInfo(const QString & fn);

private:
    QString preFn_s;
    QString clsName_s;
    QString fnName_s;
    QString postFn_s;
    QStringList fnArgs_qsl;
    QString tplInfo_s;
    QString fi_s;
    int ln_i;
    QString fullFn_s;
    QVariant return_var;
    QElapsedTimer _et;
    static QList<QThread *> depth_threadList;
};

#endif // LOGFUNCTION_H
