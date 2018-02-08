#ifndef VARIABLEHEADER_H
#define VARIABLEHEADER_H
#include "eirTypes.h"

#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QPair>

#include "VariableId.h"
class VariableGroup;
class VariableType;

typedef QPair<VariableId, VariableType *> VariableColumn;

class EIRTYPESSHARED_EXPORT VariableHeader
{
public:
    VariableHeader(void);
    VariableHeader(VariableGroup * vg);
    bool isNull(void) const;
    int size(void) const;
    VariableId id(int index) const;
    QMetaType::Type metaType(int index) const;
    QList<VariableColumn> header(void) const;

private:
    QList<VariableColumn> column_list;
};

#endif // VARIABLEHEADER_H
