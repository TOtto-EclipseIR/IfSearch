#ifndef OBJECTVARIABLEGROUP_H
#define OBJECTVARIABLEGROUP_H
#include "eirVariable.h"


#include "VariableGroup.h"

class EIRVARIABLESHARED_EXPORT ObjectVariableGroup : public VariableGroup
{
public:
    explicit ObjectVariableGroup(QObject * object,
                                 const QString & group=QString());

private:
    QObject * _object;
};

#endif // OBJECTVARIABLEGROUP_H
