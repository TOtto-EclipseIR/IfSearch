/*! @file TypeBehavior.cpp Definitions for TypeBehavior base class
*
*/
#include "TypeBehavior.h"

#include <QDomElement>
#include <QVariant>
#include <QVector>

#include <eirBase/BaseLog.h>

QVector<int> TypeBehavior::size_digits_i_vec
    = QVector<int>() << -1 << 3 << 5 << 8 << 10 << 0 << 0 << 0 << 20;
/*                       0    1    2    3     4    5    6    7     8 */

TypeBehavior::TypeBehavior(void)
{
}

QString TypeBehavior::readable(const QVariant & var)
{
    BASSERT(var.canConvert(QVariant::String));
    return var.toString();
}

QString TypeBehavior::sortable(const QVariant & var)
{
    BASSERT(var.canConvert(QVariant::String));
    return var.toString().toLower();
}

QStringList TypeBehavior::hexdump(const QVariant & var,
                            const QString & title)
{
    BFUNCTION(var, title);
    (void)var;
    (void)title;
    BMUSTDO("hexdump()");
    return QStringList();
}

QString TypeBehavior::parsable(const QVariant & var)
{
    (void)var;
    BMUSTDO("parsable()");
    return QString();
}


QVariant TypeBehavior::parsed(const int dtype,
                              const QString & s)
{
    (void)s;
    (void)dtype;
    BMUSTDO("parsed()");
    return QVariant();
}


QDomElement TypeBehavior::domVariant(const QVariant & var)
{
    (void)var;
    BMUSTDO("domVariant()");
    return QDomElement();
}


QVariant TypeBehavior::variantDom(const int dtype,
                                  const QDomElement & de)
{
    (void)dtype;
    (void)de;
    BMUSTDO("variantDom()");
    return QVariant();
}


QStringList TypeBehavior::binaryEncoded(const int dtype,
                                        const QByteArray & ba)
{
    (void)dtype;
    (void)ba;
    BMUSTDO("binaryEncoded()");
    return QStringList();
}


QByteArray TypeBehavior::binaryDecoded(const int dtype,
                                       const QStringList & qsl)
{
    (void)dtype;
    (void)qsl;
    BMUSTDO("binaryDecoded()");
    return QByteArray();
}


QByteArray TypeBehavior::baVariant(const QVariant & var)
{
    (void)var;
    BMUSTDO("baVariant()");
    return QByteArray();
}


QVariant TypeBehavior::variantBA(const int dtype, const QByteArray & ba)
{
    (void)dtype;
    (void)ba;
    BMUSTDO("variantBA()");
    return QVariant();
}
