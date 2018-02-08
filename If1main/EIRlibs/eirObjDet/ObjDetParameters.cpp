#include "ObjDetParameters.h"

ObjDetParameters::ObjDetParameters(QSettings * settings,
                                   const QString & group)
     : SettingsVariableGroup(settings, group)
{
}
