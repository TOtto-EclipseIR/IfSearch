#ifndef FRAMESOURCESTATUSINDEX_H
#define FRAMESOURCESTATUSINDEX_H
#include "eirFrameSource.h"

#include "../eirBase/Enumeration.h"

#define FRAMESOURCESTATUSINDEX_ENUM(NV) \
    NV(Initialization, = 0)  \
    NV(FirstConnection,)   \
    NV(LastConnection,)   \
    NV(Configured,)   \
    NV(LastStarted,)   \
    NV(Pause,)   \
    NV(Finished,)   \
    NV(Error,)   \
    NV(UserBegin, = 31) \
    NV(UserEnd, = 63) \

/* Plugins inheriting FrameSourcePlugin should register indexes
   between UserBegin+1 and UserEnd-1.
   */


class EIRFRAMESOURCESHARED_EXPORT FrameSourceStatusIndex
        : public Enumeration
{
    DECLARE_ENUMERATION(FrameSourceStatusIndex,
                        FRAMESOURCESTATUSINDEX_ENUM)
};

#endif // FRAMESOURCESTATUSINDEX_H
