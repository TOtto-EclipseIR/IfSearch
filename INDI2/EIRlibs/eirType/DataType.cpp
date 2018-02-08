/*! @file Type.cpp Definitions for Type class
*
*/
#include "DataType.h"

#include <QMetaType>
#include <QDomElement>

#include <eirBase/BaseLog.h>

#include "TypeBehavior.h"

const QString DataType::attribute_s("DataType");
const QString DataType::elementTag_s("eirType");
QMap<int, TypeBehavior *> DataType::type_behavior_map;

DataType::DataType(void)
    : type_i(0)
    , name_s("invalid")
    , behavior_p(0)
{
}

DataType::DataType(const QString & name)
    : type_i(0)
    , name_s("invalid")
    , behavior_p(0)
{
    set(name);
}

DataType::DataType(const QMetaType::Type mtype)
    : type_i(mtype)
    , name_s(QMetaType::typeName(mtype))
    , behavior_p(behavior())
{
}

DataType::DataType(const QVariant & var)
    : type_i(var.type())
    , name_s(var.typeName())
    , behavior_p(behavior())
{
}

bool DataType::isValid(void) const
{
    if (QMetaType::typeName(type_i))    return true;
    if (QVariant::typeToName(type_i))   return true;
    return false;
}

QString DataType::name(void) const
{
    return name_s;
}

DataType::operator int(void) const
{
    return type_i;
}

bool DataType::set(const QString & name)
{
    int mt = QMetaType::type(qPrintable(name));
    if (QMetaType::UnknownType != mt)
    {
        type_i = mt;
        name_s = QMetaType::typeName(mt);
        behavior_p = behavior();
        return true;
    }

    QVariant::Type vt = QVariant::nameToType(qPrintable(name));
    if (QVariant::Invalid != vt)
    {
        type_i = vt;
        name_s = QVariant::typeToName(vt);
        behavior_p = behavior();
        return true;
    }
    return false;
}

TypeBehavior * DataType::behavior(void)
{
    if (type_behavior_map.contains(type_i))
        return type_behavior_map.value(type_i);

    BMUSTDO(QString(name()));
    return 0;
}

void DataType::addBehavior(const int metaType,
                           TypeBehavior * behavior)
{
    if (QMetaType::UnknownType == metaType)
    {    BMUSTDO(metaType); }
    else if ( ! QMetaType::isRegistered(metaType))
    {    BMUSTDO(metaType); }
    else
        type_behavior_map.insert(metaType, behavior);
}


void DataType::addBehavior(const char * typeName,
                           TypeBehavior * behavior)
{
    int metaType = QMetaType::type(typeName);
    if (QMetaType::UnknownType == metaType)
    {    BMUSTDO(typeName); }
    else if ( ! QMetaType::isRegistered(metaType))
    {    BMUSTDO(typeName); }
    else
        type_behavior_map.insert(metaType, behavior);
}

QString DataType::name(const int i)
{
    QString result;
    const char * pc = QMetaType::typeName(i);
    if (pc) return result = pc;
    pc = QVariant::typeToName(i);
    if (pc) return result = pc;
    return result;
}

QString DataType::readable(const QVariant & var) const
{
    return behavior_p
            ? behavior_p->readable(var)
            : QString();
}

QString DataType::sortable(const QVariant & var) const
{
    return behavior_p
            ? behavior_p->sortable(var)
            : QString();
}

QStringList DataType::hexdump(const QVariant & var,
                              const QString & title) const
{
    return behavior_p
            ? behavior_p->hexdump(var, title)
            : QStringList();

}

QString DataType::parsable(const QVariant & var) const
{
    return behavior_p
            ? behavior_p->parsable(var)
            : QString();
}

QVariant DataType::parsed(const int dtype, const QString & s) const
{
    return behavior_p
            ? behavior_p->parsed(dtype, s)
            : QVariant();
}

QDomElement DataType::domVariant(const QVariant & var) const
{
    return behavior_p
            ? behavior_p->domVariant(var)
            : QDomElement();
}

QVariant DataType::variantDom(const int dtype, const QDomElement & de) const
{
    return behavior_p
            ? behavior_p->variantDom(dtype, de)
            : QVariant();
}
