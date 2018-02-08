#include "StringBehavior.h"

#include <QDomElement>

#include <eirBase/BaseLog.h>
#include <eirKID/BasicId.h>

#include "Hexdump.h"

StringBehavior * StringBehavior::instance_p = 0;

StringBehavior * StringBehavior::instance(void)
{
    if ( ! instance_p) instance_p = new StringBehavior;
    return instance_p;
}

StringBehavior::StringBehavior(void)
{
}

QString StringBehavior::sortable(const QVariant & var)
{
    BFUNCTION(var);
    QString result(var.toString().toLower());
    BFNRETURN(result);
    return result;
}

QStringList StringBehavior::hexdump(const QVariant & var,
                                     const QString & title)
{
    BFUNCTION(var, title);
    QString s = var.toString();
    QByteArray ba = s.toLocal8Bit();
    QStringList result(Hexdump::from(ba, title));
    BFNRETURN(result);
    return result;
}

QString StringBehavior::parsable(const QVariant & var)
{
    BFUNCTION(var);
    QString result = QString("%1 %2")
            .arg(DataType::name(QMetaType::type("BasicId")))
            .arg(var.toString());
    BFNRETURN(result);
    return result;
}

QVariant StringBehavior::parsed(const QString & s)
{
    BFUNCTION(s);
    QString is = s.simplified();
    int x = is.indexOf(' ');
    if (x > 0) is = is.mid(x+1);
    BFNRETURN(is);
    return QVariant(is);
}

QDomElement StringBehavior::domVariant(const QVariant & var)
{
    BFUNCTION(var);
    QDomDocument tempDoc;
    QDomElement result = tempDoc.createElement(DataType::elementTag_s);
    result.setAttribute(DataType::attribute_s,
                        QMetaType::typeName(QMetaType::type("BasicId")));
    QString text = var.toString();
    QDomText textNode = result.ownerDocument().createTextNode(text);
    result.appendChild(textNode);
    BFNRETURN(result.text());
    return result;
}

QVariant StringBehavior::variantDom(const QDomElement & de)
{
    BFUNCTION(de.text());
    BEXPECTEQ(QMetaType::typeName(QMetaType::type("BasicId")),
                  de.attribute(DataType::attribute_s));
    QString s = de.text();
    BFNRETURN(s);
    return QVariant(s);
}
