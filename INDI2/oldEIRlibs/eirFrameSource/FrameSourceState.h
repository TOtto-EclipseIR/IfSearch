#ifndef FRAMESOURCESTATES_H
#define FRAMESOURCESTATES_H
#include "eirFrameSource.h"

#include "../eirBase/Enumeration.h"

#define FRAMESOURCESTATE_ENUM(NV) \
    NV(Construct, = 0)  \
    NV(Initialized,)   \
    NV(Connecting,)   \
    NV(Connected,)   \
    NV(Configuring,) \
    NV(Configured,)   \
    NV(Starting,)   \
    NV(Running,)   \
    NV(Ready,)   \
    NV(Grabbing,)   \
    NV(Retry, ) \
    NV(Paused,)   \
    NV(Finished,)   \
    NV(Error,)   \


class EIRFRAMESOURCESHARED_EXPORT FrameSourceState
        : public Enumeration
{
    DECLARE_ENUMERATION(FrameSourceState,
                        FRAMESOURCESTATE_ENUM);
};

#endif // FRAMESOURCESTATES_H
