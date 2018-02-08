#ifndef VARIABLETABLE_H
#define VARIABLETABLE_H
#include "eirCore.h"

#include "VariableHeader.h"
#include "../eirBase/Matrix.h"

#include <QtCore/QVariant>

class EIRCORESHARED_EXPORT VariableTable
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
