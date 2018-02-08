#include "AverageFace.h"

AverageFace::AverageFace(QSize size, Eyes eyes)
{
    Data.fill(size.isValid() ? size : QSize(256,256), AnyColor(RGB));
    NormalEyes = eyes.isNull() ? eyes
        : Eyes::fromSizeDistance(Data.size(), Data.width() / 4);
} // ctor()

void AverageFace::add(QImage image, Eyes eyes)
{
    // TODO: Normalize image if eyes don't match
    if (eyes == NormalEyes)
        Data += AnyColorMatrix(RGB, image);
} // add()


QImage AverageFace::average(void) const
{
    return Data.toImage();
} // average()


