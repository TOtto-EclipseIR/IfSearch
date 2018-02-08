#ifndef SDKCLIENTPARAMETERS_H
#define SDKCLIENTPARAMETERS_H
#include "eirSDK1.h"

#include "../eirCore/VariableSet.h"

#define SDKCLIENTPARAMETERS_VARIABLES(TIVD) \
    TIVD(QString, Binary_ExeName, QString, QString("IfSearch.exe")) \
    TIVD(QString, Binary_PathName, QString, QString(".")) \
    TIVD(QString, Binary_ExeArgs, QString, QString()) \
    TIVD(int, Settings_UpdateMsec, int, 1000) \
    TIVD(int, Initialize_TimeoutMsec, int, 180000) \


class EIRSDK1SHARED_EXPORT SdkClientParameters : public VariableSet
{
public:
    DECLARE_VARIABLESET(SDKCLIENTPARAMETERS_VARIABLES);
    explicit SdkClientParameters(void);
};

#endif // SDKCLIENTPARAMETERS_H
