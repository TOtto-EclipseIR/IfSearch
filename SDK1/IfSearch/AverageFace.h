#ifndef AVERAGEFACE_H
#define AVERAGEFACE_H

#include <QImage>
#include <QSize>

#include <Eyes.h>
#include <AnyColorMatrix.h>

class AverageFace
{
public:
    AverageFace(QSize size=QSize(), Eyes eyes=Eyes());
    void add(QImage image, Eyes eyes);
    QImage average(void) const;

private:
    Eyes NormalEyes;
    AnyColorMatrix Data;
};

#endif // AVERAGEFACE_H
