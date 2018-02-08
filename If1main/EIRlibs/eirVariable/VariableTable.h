#ifndef VARIABLETABLE_H
#define VARIABLETABLE_H
#include "eirVariable.h"

#include "VariableHeader.h"
#include "../eirCollection/Matrix.h"

#include <QtCore/QVariant>

class EIRVARIABLESHARED_EXPORT VariableTable
{
public:
    VariableTable(void);
    void setHeader(VariableHeader header);
    void loadData(Matrix<QVariant> matrix);
    int rowCount(void) const;
    bool readRow(VariableGroup * vg, int row);

private:
    VariableHeader variable_header;
    Matrix<QVariant> variant_matrix;
};

#endif // VARIABLETABLE_H
