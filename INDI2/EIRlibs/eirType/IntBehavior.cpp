/*! @file IntBehavior.cpp Definitions for IntBehavior class
*
*/
#include "IntBehavior.h"

#include <QDomElement>
#include <QVector>

#include <eirBase/BaseLog.h>

#include "DataType.h"
#include "Hexdump.h"

IntBehavior * IntBehavior::instance_p = 0;

IntBehavior * IntBehavior::instance(void)
{
    if ( ! instance_p) instance_p = new IntBehavior;
    return instance_p;
}

IntBehavior::IntBehavior(void)
{
}

QString IntBehavior::sortable(const QVariant & var)
{
    QString result;
    int i = var.toInt();
    int s = sizeof(int);
    int d = size_digits_i_vec.at(s);

    if (i < 0)
        result = QString("%1").arg(i, d+1, 10, QChar('0'));
    else if (i > 0)
        result = QString("+%1").arg(i, d, 10, QChar('0'));
    else
        result.fill(QChar('0'), d+1);

    return result;
}

QStringList IntBehavior::hexdump(const QVariant & var,
                                     const QString & title)
{
    BFUNCTION(var, title);
    int i = var.toInt();
    QByteArray ba((const char *)&i, sizeof(i));
    QStringList result(Hexdump::from(ba, title));
    BFNRETURN(result.size());
    return result;
}

QString IntBehavior::parsable(const QVariant & var)
{
    BFUNCTION(var);
    return QString("%1 %2")
            .arg(DataType::name(QMetaType::Int))
            .arg(var.toInt());
}

QVariant IntBehavior::parsed(const int dtype,
                             const QString & s)
{
    BFUNCTION(s);
    bool ok;
    QVariant result;
    long long int i;
    long long unsigned u;

    switch (dtype)
    {
    case QMetaType::Char:
    case QMetaType::Int:
    case QMetaType::Short:
    case QMetaType::Long:
    case QMetaType::LongLong:
        i = s.toLongLong(&ok);
        if (ok) result.setValue(i);
        break;

    case QMetaType::UChar:
    case QMetaType::UInt:
    case QMetaType::UShort:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        u = s.toULongLong(&ok);
        if (ok) result.setValue(u);
        break;
    default:
        BMUSTDO(QString::number(dtype));
        break;
    }
    result.convert(dtype);
    BFNRETURN(result);
    return result;
}

QDomElement IntBehavior::domVariant(const QVariant & var)
{
    BFUNCTION(var);
    QDomDocument tempDoc;
    QDomElement result = tempDoc.createElement(DataType::elementTag_s);
    result.setAttribute(DataType::attribute_s,
                        QMetaType::typeName(QMetaType::Int));
    QString text = QString::number(var.toInt());
    QDomText textNode = result.ownerDocument().createTextNode(text);
    result.appendChild(textNode);
    BFNRETURN(result.text());
    return result;
}

QVariant IntBehavior::variantDom(const int dtype, const QDomElement & de)
{
    BFUNCTION(de.text());
    QVariant result(parsed(dtype, de.text()));
    BFNRETURN(result);
    return result;
}
