/*! @file DataType.h Declarations for Type class
*
*/
#ifndef TYPE_H
#define TYPE_H
#include "eirType.h"

#include <QMap>
#include <QMetaType>
#include <QVariant>

class QDomElement;
class QString;

class TypeBehavior;

class EIRTYPESHARED_EXPORT DataType
{
public:
    DataType(void);
    DataType(const int i);
    DataType(const QString & name);
    DataType(const QMetaType & meta);
    DataType(const QMetaType::Type mtype);
    DataType(const QVariant & var);
    DataType(const QVariant::Type vtype);

    bool isValid(void) const;
    bool isNull(void) const;
    bool isVarType(void) const;
    bool isMetaType(void) const;
    QString name(void) const;
    bool set(const QString & name);
    operator int(void) const;

    TypeBehavior * behavior(void);
    void setBehavior(TypeBehavior * p);

    QString readable(const QVariant & var) const;
    QString sortable(const QVariant & var) const;
    QStringList hexdump(const QVariant & var,
                        const QString & title=QString()) const;
    QString parsable(const QVariant & var) const;
    QVariant parsed(const int dtype, const QString & s) const;
    QDomElement domVariant(const QVariant & var) const;
    QVariant variantDom(const int dtype, const QDomElement & de) const;
    QStringList binaryEncoded(const QByteArray & ba) const;
    QByteArray binaryDecoded(const QStringList & qsl) const;
    QByteArray baVariant(const QVariant & var) const;
    QVariant variantBA(const QByteArray & ba) const;

public:
    static QString name(const int i);
    static void addBehavior(const int metaType,
                            TypeBehavior * behavior);
    static void addBehavior(const char *typeName,
                            TypeBehavior * behavior);

private:
    bool ctor(const int i,
              const QString & name);
public:
    static const QString attribute_s;
    static const QString elementTag_s;

private:
    int type_i;
    QString name_s;
    TypeBehavior * behavior_p;

private:
    static QMap<int, TypeBehavior *> type_behavior_map;
};

#endif // TYPE_H
