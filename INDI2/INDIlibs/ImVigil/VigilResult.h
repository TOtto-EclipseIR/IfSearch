#ifndef VIGILRESULT_H
#define VIGILRESULT_H
#include "ImVigil.h"

#include "VigilSDK.h"

class VariableSet;

class IMVIGILSHARED_EXPORT VigilResult
{
public:
    VigilResult(VigilSDK::_CVigilResult * result);
    bool isOk(void) const;
    QString siteName(void) const;
    int cameraCount(void) const;
    QString cameraName(int channelIndex);
    VariableSet loginResult(void) const;

private:
    VigilSDK::_CVigilResult * _result;
};

#endif // VIGILRESULT_H
