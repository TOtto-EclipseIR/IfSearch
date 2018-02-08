/*! @file Parser.cpp Definitions for Parser class
*
*/
#include "Parsed.h"

#include "DataType.h"

Parsed::Parsed(const QString & s)
{
    QString ss = s.simplified();
    int x = ss.indexOf(' ');
    if (x > 0)
    {
        QString name = ss.left(x);
        QString value = ss.mid(x+1);
        DataType dt;
        if (dt.set(name))
            QVariant::setValue(dt.parsed(dt, value));
    }
}

Parsed::operator QVariant (void)
{
    return QVariant(*this);
}
