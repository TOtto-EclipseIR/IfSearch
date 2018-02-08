#include "Variable.h"

#include <QtXml/QDomElement>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QSizeF>

#include "VariableType.h"

Variable::Variable(void)
{
}

Variable::Variable(const VariableId & id,
                   const QMetaType::Type varType) :
    _id(id),
    _behavior(VariableType::behavior(varType)),
    current_var(QVariant(varType)),
    default_var(QVariant(varType))
{
}

Variable::Variable(const VariableId & id,
                   const QVariant defaultValue) :
    _id(id),
    _behavior(VariableType::behavior((QMetaType::Type)defaultValue.userType())),
    current_var(defaultValue),
    default_var(defaultValue)
{
}

Variable::Variable(const VariableId & id,
         const QVariant currentValue,
         const QVariant defaultValue) :
    _id(id),
    _behavior(VariableType::behavior((QMetaType::Type)defaultValue.userType())),
    current_var(currentValue),
    default_var(defaultValue)
{
}

Variable Variable::fromString(const QString & string)
{
    int x = string.indexOf('=');
    if (x < 0)
        return Variable(VariableId(string), true);
    else
    {
        QString name = string.left(x);
        QString value = string.mid(x+1);
        return Variable(VariableId(name), value);
    }
}

bool Variable::isNull(void) const
{
    if (_id.isNull()) return true;
    if ( ! _behavior) return true;
    if (default_var.isNull()) return true;
    return false;
}

VariableId Variable::id(void) const
{
    return _id;
}

QVariant Variable::var(void) const
{
    return current_var;
}

QVariant Variable::defaultVar(void) const
{
    return default_var;
}

VariableType * Variable::variableType(void) const
{
    return _behavior;
}

QString Variable::typeName(void) const
{
    return QMetaType::typeName(_behavior->metaType());
}

void Variable::reset(void)
{
    current_var = default_var;
}

void Variable::clear(void)
{
    current_var = QVariant(default_var.type());
}

void Variable::set(const QVariant newValue)
{
    current_var.setValue(newValue);
}

int Variable::csvEntryCount(void) const
{
    return _behavior ? _behavior->csvColumns() : 0;
}

int Variable::csvHeadingCount(void) const
{
    int result = _id.sectionCount();
    if (_behavior && _behavior->csvParts())
        ++result;
    return result;
}

QString Variable::csvEntryString(void) const
{
    return _behavior
            ? _behavior->csvString(current_var)
            : current_var.toString();
}

QString Variable::csvEntryHeading(int row) const
{
    if (_id.isSectioned())
    {
        if (row < _id.sectionCount())
            return _id.section(row, _behavior->csvColumns());
        else if (row == _id.sectionCount())
            return _behavior ? _behavior->csvHeader() : QString();
    }
    else if (0 == row)
        return _id;

    return QString();
}
/*
bool Variable::readXml(const QDomElement & de)
{
    (void)de;
    // TODO
    return false;
}

QDomElement Variable::writeXml(void) const
{
    // TODO
    return QDomElement();
}
*/
