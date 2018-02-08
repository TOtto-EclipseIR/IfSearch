#include "Variable.h"
#include <QSharedData>

DEFINE_DATAPROPS(Variable, VariableData)

Variable::Variable(const VariableId & vid,
                   const QQVariant & value,
                   const QQVariant & dflt)
    : data(new VariableData)
{
    ctor();
    setVariableId(vid);
    setDataType(DataType(dflt));
    setCurrent(value);
    setDefault(dflt);
}

void Variable::ctor(void) {}
void Variable::dtor(void) {}

VariableId Variable::id(void) const
{
    return getVariableId();
}

void Variable::reset(void)
{
    setCurrent(getDefault());
}
