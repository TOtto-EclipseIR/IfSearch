#include "VariableTable.h"

#include <QtDebug>
#include <QtCore/QSize>

#include "Variable.h"
#include "VariableGroup.h"

VariableTable::VariableTable(void)
{
}

void VariableTable::setHeader(VariableHeader header)
{
    variable_header = header;
}

void VariableTable::loadData(Matrix<QVariant> matrix)
{
    variant_matrix = matrix;
}

int VariableTable::rowCount(void) const
{
    return variant_matrix.size().height();
}

bool VariableTable::readRow(VariableGroup * vg, int row)
{
    //qDebug() << "VariableTable::readRow()" << row;
    if (variable_header.isNull()) return false;
    if ( ! variant_matrix.hasRow(row)) return false;

    vg->clear();
    int n = variable_header.size();
    for (int x = 0; x < n; ++x)
    {
        VariableId vid(variable_header.id(x));
        QMetaType::Type mType = variable_header.metaType(x);
        QVariant mVar = variant_matrix.get(x, row);
        QVariant gVar(mType);
        if (mVar.isValid()) gVar = mVar;
        qDebug() << x << n << vid << mVar
                 << QMetaType::typeName(mType) << gVar;
        vg->set(vid, gVar);
    }
    return true;
}
