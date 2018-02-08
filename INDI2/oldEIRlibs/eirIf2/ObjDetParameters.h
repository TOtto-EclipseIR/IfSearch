#ifndef OBJDETPARAMETERS_H
#define OBJDETPARAMETERS_H

#include "../eirCore/VariableSettings.h"

class ObjDetParameters : public VariableSettings
{
public:
    explicit ObjDetParameters(QSettings * settings,
                              const QString & group=QString());
};

#endif // OBJDETPARAMETERS_H
