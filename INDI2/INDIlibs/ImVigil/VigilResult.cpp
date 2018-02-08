#include "VigilResult.h"

#include "../../EIRlibs/eirCore/VariableSet.h"

VigilResult::VigilResult(VigilSDK::_CVigilResult * result)
{
    _result = result;
}

bool VigilResult::isOk(void) const
{
    return "OK" == _result->HResult();
}

QString VigilResult::siteName(void) const
{
    return _result->SiteName();
}

int VigilResult::cameraCount(void) const
{
    return _result->CameraCount();
}

QString VigilResult::cameraName(int channelIndex)
{
    return _result->GetCameraName(channelIndex);
}

VariableSet VigilResult::loginResult(void) const
{
    VariableSet result("loginResult");
    result.set("LoggedOn", _result->LoggedOn());
    result.set("MinClientVersion", QString("%1.%2.%3")
               .arg(_result->MinClientVersionMajor())
               .arg(_result->MinClientVersionMinor())
               .arg(_result->MinClientVersionRevision()));
    result.set("ServerVersion", QString("%1.%2.%3")
               .arg(_result->ServerVersionMajor())
               .arg(_result->ServerVersionMinor())
               .arg(_result->ServerVersionRevision()));
    result.set("SiteName", _result->SiteName());
    result.set("POSRuning", _result->POSRunning());
    result.set("AdjustableClientFeed", _result->AdjustableClientFeed());
    result.set("CameraCount", _result->CameraCount());
    result.set("LimitSearchToOneDay", _result->LimitSearchToOneDay());
    result.set("DVRBoard", _result->DVRBoard());
    result.set("HResult", _result->HResult());
    return result;
}
