#ifndef VARIABLETYPEBEHAVIOR_H
#define VARIABLETYPEBEHAVIOR_H
#include "eirCore.h"

#include <QMap>
#include <QMetaType>
#include <QString>
#include <QStringList>

class QDomElement;
class QVariant;

class VariableType
{
protected:
    VariableType(QMetaType::Type varType,
                         const QString & partNames=QString());

public:
    static VariableType * behavior(const QMetaType::Type metaType);
    QMetaType::Type metaType(void) const;
    virtual int csvColumns(void) const;
    virtual int csvParts(void) const;
    virtual QString csvHeader(void) const;
    virtual QString csvString(const QVariant & var) const;
    //virtual QVariant readXml(const QDomElement & de) const;
    //virtual QDomElement writeXml(const QVariant & var) const;

private:
    QMetaType::Type var_type;
    QStringList partName_list;
    static QMap<QMetaType::Type, VariableType *> behavior_map;
};

#endif // VARIABLETYPEBEHAVIOR_H
