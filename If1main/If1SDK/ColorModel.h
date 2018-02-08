/*! \file ColorModel.h Declarations for ColorModel NamedEnum
  *
  */

#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <NamedEnum2.h>

#define COLORMODEL_ENUM(NV) \
    NV(Null,)               \
    NV(RGB,)                \
    NV(HSV,)                \
    NV(CMYK,)               \
    NV(HLS,)                \

DECLARE_NAMEDENUM(ColorModel, , COLORMODEL_ENUM)

#endif // COLORMODEL_H
