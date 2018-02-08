#ifndef OBJDETPARAMETERS_H
#define OBJDETPARAMETERS_H

#include "../eirVariable/SettingsVariableGroup.h"

class ObjDetParameters : public SettingsVariableGroup
{
public:
    explicit ObjDetParameters(QSettings * settings,
                              const QString & group=QString());
};

#endif // OBJDETPARAMETERS_H
