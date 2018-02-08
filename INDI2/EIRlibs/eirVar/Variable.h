#ifndef VARIABLE_H
#define VARIABLE_H
#include "eirVar.h"

#include <QSharedDataPointer>

#include <eirBase/DataProperty.h>
#include <eirType/DataType.h>
#include <eirType/QQVariant.h>
#include <eirKID/BasicId.h>

typedef BasicId VariableId;
typedef QList<VariableId> VariableIdList;

#define VARIABLE_DATAPROPS(TND) \
    TND(VariableId, VariableId, VariableId()) \
    TND(DataType,   DataType,   DataType()) \
    TND(QQVariant,  Current,    QQVariant()) \
    TND(QQVariant,  Default,    QQVariant()) \

class VariableData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(VARIABLE_DATAPROPS)
public:
    VariableData(void)
    {
        DEFINE_DATAPROPS_CTORS(VARIABLE_DATAPROPS)
    }
};

class EIRVARSHARED_EXPORT Variable
{
    DECLARE_PARENT_DATAPROPS(VARIABLE_DATAPROPS)
    DECLARE_DATAPROPS(Variable, VariableData)
public:
    Variable(const VariableId & vid,
             const QQVariant & value,
             const QQVariant & dflt);

public:
    VariableId id(void) const;
    void reset(void);
};
Q_DECLARE_TYPEINFO(Variable, Q_PRIMITIVE_TYPE);
Q_DECLARE_METATYPE(Variable)

typedef QList<Variable> VariableList;

#endif // VARIABLE_H
