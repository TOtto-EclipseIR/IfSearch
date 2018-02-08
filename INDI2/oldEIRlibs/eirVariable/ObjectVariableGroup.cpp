#include "ObjectVariableGroup.h"

ObjectVariableGroup::ObjectVariableGroup(QObject * object,
                                         const QString & group) :
    VariableGroup(group),
    _object(object)
{
}
