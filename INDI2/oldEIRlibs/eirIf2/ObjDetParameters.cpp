#include "ObjDetParameters.h"

ObjDetParameters::ObjDetParameters(QSettings * settings,
                                   const QString & group)
     : VariableSettings(settings, group)
{
}
