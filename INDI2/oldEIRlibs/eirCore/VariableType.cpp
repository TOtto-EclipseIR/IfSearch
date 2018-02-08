#include "VariableType.h"

#include <QtDebug>
#include <QtCore/QLineF>
#include <QtCore/QPointF>
#include <QtCore/QLineF>
#include <QtCore/QSizeF>
#include <QtCore/QVariant>
#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtXml/QDomElement>

class VariableVoidBehavior : public VariableType
{
public:
    VariableVoidBehavior(QMetaType::Type metaType=QMetaType::Void);
};

class VariableDoubleBehavior : public VariableType
{
public:
    VariableDoubleBehavior(QMetaType::Type metaType=QMetaType::Double);
};

class VariableIntBehavior : public VariableType
{
public:
    VariableIntBehavior(QMetaType::Type metaType=QMetaType::Int);
};

class VariableBoolBehavior : public VariableType
{
public:
    VariableBoolBehavior(QMetaType::Type metaType=QMetaType::Bool);
    virtual QString csvString(const QVariant & var) const;
};

class VariableQCharBehavior : public VariableType
{
public:
    VariableQCharBehavior(QMetaType::Type metaType=QMetaType::QChar);
    virtual QString csvString(const QVariant & var) const;
};

class VariablePointBehavior : public VariableType
{
public:
    VariablePointBehavior(QMetaType::Type metaType=QMetaType::QPoint);
    virtual QString csvString(const QVariant & var) const;
};

class VariableSizeBehavior : public VariableType
{
public:
    VariableSizeBehavior(QMetaType::Type metaType=QMetaType::QSize);
    virtual QString csvString(const QVariant & var) const;
};

class VariableLineBehavior : public VariableType
{
public:
    VariableLineBehavior(QMetaType::Type metaType=QMetaType::QLine);
    virtual QString csvString(const QVariant & var) const;
};

class VariableRectBehavior : public VariableType
{
public:
    VariableRectBehavior(QMetaType::Type metaType=QMetaType::QRect);
    virtual QString csvString(const QVariant & var) const;
};

class VariableImageBehavior : public VariableType
{
public:
    VariableImageBehavior(QMetaType::Type metaType=QMetaType::QImage);
    virtual QString csvString(const QVariant & var) const;
};

class VariableStringBehavior : public VariableType
{
public:
    VariableStringBehavior(QMetaType::Type metaType=QMetaType::QString);
    virtual QString csvString(const QVariant & var) const;
};

class VariableStringListBehavior : public VariableType
{
public:
    VariableStringListBehavior(QMetaType::Type metaType=QMetaType::QStringList);
    virtual QString csvString(const QVariant & var) const;
};

class VariableColorBehavior : public VariableType
{
public:
    VariableColorBehavior(QMetaType::Type metaType=QMetaType::QColor);
    virtual QString csvString(const QVariant & var) const;
};


QMap<QMetaType::Type, VariableType *> VariableType::behavior_map;

VariableType * VariableType::behavior(const QMetaType::Type metaType)
{
    if (behavior_map.contains(metaType))
        return behavior_map.value(metaType);

    switch(metaType)
    {
    case QMetaType::Void:       return new VariableVoidBehavior(metaType);
    case QMetaType::Bool:       return new VariableBoolBehavior(metaType);
    case QMetaType::QChar:       return new VariableQCharBehavior(metaType);
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::Int:        return new VariableIntBehavior(metaType);
    case QMetaType::Double:     return new VariableDoubleBehavior(metaType);
    case QMetaType::QString:    return new VariableStringBehavior(metaType);
    case QMetaType::QStringList: return new VariableStringListBehavior(metaType);
    case QMetaType::QColor:     return new VariableColorBehavior(metaType);
    case QMetaType::QPointF:
    case QMetaType::QPoint:      return new VariablePointBehavior(metaType);
    case QMetaType::QSizeF:
    case QMetaType::QSize:      return new VariableSizeBehavior(metaType);
    case QMetaType::QLineF:
    case QMetaType::QLine:      return new VariableLineBehavior(metaType);
    case QMetaType::QRectF:
    case QMetaType::QRect:      return new VariableRectBehavior(metaType);
    case QMetaType::QImage:     return new VariableImageBehavior(metaType);
    default:                    qFatal("Unhandled QMetaType: %i %s", metaType,
                                         QMetaType::typeName(metaType));
                                return 0;
    }
}

VariableType::VariableType(QMetaType::Type varType,
                           const QString & partNames) :
    var_type(varType),
    partName_list(partNames.simplified().split(' ', QString::SkipEmptyParts))
{
    behavior_map.insert(varType, this);
}

QMetaType::Type VariableType::metaType(void) const
{
    return var_type;
}

int VariableType::csvColumns(void) const
{
    return partName_list.isEmpty() ? 1 : partName_list.size();
}

int VariableType::csvParts(void) const
{
    return partName_list.isEmpty() ? 0 : partName_list.size();
}

QString VariableType::csvHeader(void) const
{
    return '"' + partName_list.join("\",\"") + '"';
}

QString VariableType::csvString(const QVariant & var) const
{
    return var.toString();
}

VariableVoidBehavior::VariableVoidBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

VariableDoubleBehavior::VariableDoubleBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

VariableIntBehavior::VariableIntBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

VariablePointBehavior::VariablePointBehavior(QMetaType::Type metaType)
    : VariableType(metaType, "x y")
{
}

QString VariablePointBehavior::csvString(const QVariant & var) const
{
    QPointF pt = var.toPointF();
    return QString("%1,%2").arg(pt.x()).arg(pt.y());
}

VariableSizeBehavior::VariableSizeBehavior(QMetaType::Type metaType)
    : VariableType(metaType, "width height")
{
}

QString VariableSizeBehavior::csvString(const QVariant & var) const
{
    QSizeF sz = var.toSizeF();
    return QString("%1,%2").arg(sz.width()).arg(sz.height());
}

VariableLineBehavior::VariableLineBehavior(QMetaType::Type metaType)
    : VariableType(metaType, "x1 y1 x2 y2")
{
}

QString VariableLineBehavior::csvString(const QVariant & var) const
{
    QLineF ln = var.toLineF();
    return QString("%1,%2,%3,%4")
            .arg(ln.x1()).arg(ln.y1())
            .arg(ln.x2()).arg(ln.y2());
}

VariableRectBehavior::VariableRectBehavior(QMetaType::Type metaType)
    : VariableType(metaType, "x y width height")
{
}

QString VariableRectBehavior::csvString(const QVariant & var) const
{
    QRectF rt = var.toRectF();
    return QString("%1,%2,%3,%4")
            .arg(rt.center().x())
            .arg(rt.center().y())
            .arg(rt.width()).arg(rt.height());
}

VariableImageBehavior::VariableImageBehavior(QMetaType::Type metaType)
    : VariableType(metaType, "cols rows")
{
}

QString VariableImageBehavior::csvString(const QVariant & var) const
{
    QSize sz = var.value<QImage>().size();
    return QString("%1,%2").arg(sz.width()).arg(sz.height());
}

VariableBoolBehavior::VariableBoolBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

QString VariableBoolBehavior::csvString(const QVariant & var) const
{
    return var.toBool() ? "true " : "false";
}

VariableQCharBehavior::VariableQCharBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

QString VariableQCharBehavior::csvString(const QVariant & var) const
{
    return '\'' + var.toChar() + '\'';
}

VariableStringBehavior::VariableStringBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

QString VariableStringBehavior::csvString(const QVariant & var) const
{
    return '"' + var.toString() + '"';
}

VariableStringListBehavior::VariableStringListBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

QString VariableStringListBehavior::csvString(const QVariant & var) const
{
    return '"' + var.toStringList().join(";") + '"';
}

VariableColorBehavior::VariableColorBehavior(QMetaType::Type metaType)
    : VariableType(metaType)
{
}

QString VariableColorBehavior::csvString(const QVariant & var) const
{
    return "\"" + var.value<QColor>().name() + "\"";
}

