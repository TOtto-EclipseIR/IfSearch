#ifndef IMAGECLASS_H
#define IMAGECLASS_H
#include "eirImage.h"

#include "../eirBase/Enumeration.h"

#define IMAGECLASS_ENUM(NV) \
    NV(Null, = 0)  \
    NV(Frame,) \
    NV(Message,) \
    NV(DetectedFace,) \
    NV(NormalizedFace,) \
    NV(MarkedFace,) \
    NV(MarkedFrame,) \

class EIRIMAGESHARED_EXPORT ImageClass : public Enumeration
{
    DECLARE_ENUMERATION(ImageClass, IMAGECLASS_ENUM)
};

#endif // IMAGECLASS_H
