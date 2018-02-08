#ifndef FRAMESOURCERESULT_H
#define FRAMESOURCERESULT_H
#include "eirFrameSource.h"

#include "../eirExe/ResultInfo.h"

class EIRFRAMESOURCESHARED_EXPORT FrameSourceResult
{
public:
    const static ResultCode Initializing;
    const static ResultCode Initialized;
    const static ResultCode Connecting;
    const static ResultCode Connected;
    const static ResultCode ConnectError;
    const static ResultCode Started;
    const static ResultCode StartError;
    const static ResultCode PlaybackComplete;
    const static ResultCode HttpConnectSuccess;
    const static ResultCode HttpConnectFailure;
    const static ResultCode LiveDirConnectFailure;
};

#endif // FRAMESOURCERESULT_H
