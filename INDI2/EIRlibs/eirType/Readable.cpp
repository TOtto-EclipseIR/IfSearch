/*! @file Readable.cpp Definitions for Readable class
*
*/
#include "Readable.h"

#include <QVariant>

#include "DataType.h"

Readable::Readable(const QVariant &var)
{
    DataType typ(var);
    QString::append(typ.readable(var));
}

Readable::operator QString (void)
{
    return QString(*this);
}

QString Readable::formatted(const QString & format,
                            const QVariantList & vars)
{
    QString result(format);
    int n = vars.size();
    QStringList sl;
    for (int x = 0; x < n; ++x)
        sl.append(Readable(vars[x]));
    n = qMin(9, n);
    for (int x = 0; x < n; ++x)
        result.replace("%"+QString::number(x), sl[x]);
    return result;
}
