#include "eirBase.h"

#include "BaseLog.h"

eirBase eirBase::instance;

eirBase::eirBase(void)
    : Library(__MODULE__)
{
}

/*! provides the next highest power of two for the specified number
 */
int eirBase::powerOfTwo(const int n)
{
    if (0 == n) return 0;
    bool neg = n < 0;
    int a = qAbs(n);
    int p = 0;
    while (a) a >>= 1, ++p;
    a = 1 << p;
    return neg ? -a : a;
}

QString eirBase::formatMessage(const QString & format,
                               const QVariantList & varList)
{
    QString result;
    int n = varList.size();
    if (format.contains("%4"))
    {
        result +=  format
                .arg((n>0) ? varList[0].toString() : QString())
                .arg((n>1) ? varList[1].toString() : QString())
                .arg((n>2) ? varList[2].toString() : QString())
                .arg((n>3) ? varList[3].toString() : QString());
        for (int x = 4; x < n; ++x)
            result += QString(" {%1}").arg(varList[x].toString());
    }
    else if (format.contains("%3"))
    {
        result +=  format
                .arg((n>0) ? varList[0].toString() : QString())
                .arg((n>1) ? varList[1].toString() : QString())
                .arg((n>2) ? varList[2].toString() : QString());
        for (int x = 3; x < n; ++x)
            result += QString(" {%1}").arg(varList[x].toString());
    }
    else if (format.contains("%2"))
    {
        result +=  format
                .arg((n>0) ? varList[0].toString() : QString())
                .arg((n>1) ? varList[1].toString() : QString());
        for (int x = 2; x < n; ++x)
            result += QString(" {%1}").arg(varList[x].toString());
    }
    else if (format.contains("%1"))
    {
        result +=  format
                .arg((n>0) ? varList[0].toString() : QString());
        for (int x = 1; x < n; ++x)
            result += QString(" {%1}").arg(varList[x].toString());
    }
    else
    {
        result +=  format;
        for (int x = 0; x < n; ++x)
            result += QString(" {%1}").arg(varList[x].toString());
    }
    return result;
}
