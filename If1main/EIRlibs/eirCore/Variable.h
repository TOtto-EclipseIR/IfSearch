#ifndef VARIABLE_H
#define VARIABLE_H
#include "eirCore.h"


#include <QObject>

#include <QVariant>

class QDomElement;

#include "VariableId.h"
class VariableType;

class EIRCORESHARED_EXPORT Variable
{
public:
    explicit Variable(void);
    Variable(const VariableId & id,
             const QMetaType::Type varType);
    Variable(const VariableId & id,
             const QVariant defaultValue);
    Variable(const VariableId & id,
             const QVariant currentValue,
             const QVariant defaultValue);
    static Variable fromString(const QString & string);
    bool isNull(void) const;
    VariableId id(void) const;
    QVariant var(void) const;
    QVariant defaultVar(void) const;
    VariableType * variableType(void) const;
    QString typeName(void) const;
    void reset(void);
    void clear(void);
    void set(const QVariant newValue);
    int csvEntryCount(void) const;
    int csvHeadingCount(void) const;
    QString csvEntryString(void) const;
    QString csvEntryHeading(int row) const;
    //bool readXml(const QDomElement & de);
    //QDomElement writeXml(void) const;

private:
    VariableId _id;
    VariableType * _behavior;
    QVariant current_var;
    QVariant default_var;
};

#endif // VARIABLE_H
