#ifndef PERSONREPORTROW_H
#define PERSONREPORTROW_H
#include "eirSDK1.h"

#include "../eirCore/VariableGroup.h"

#define PERSONREPORTROW_GROUPVARIABLES(ITD) \
    ITD(PersonKey, int, 0) \
    ITD(PersonId, QString, QString()) \
    ITD(FaceCount, int, 0) \
    ITD(Active, bool, true) \
    ITD(Description, QString, QString()) \

class EIRSDK1SHARED_EXPORT PersonReportRow : public VariableGroup
{
public:
    DECLARE_GROUPVARIABLES(PERSONREPORTROW_GROUPVARIABLES);
    PersonReportRow(void);
};

#endif // PERSONREPORTROW_H
