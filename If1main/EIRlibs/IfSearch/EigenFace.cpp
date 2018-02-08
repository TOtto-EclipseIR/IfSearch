#include <EigenFace.h>
#include "EigenFaceData.h"
#include "EigenFaceParameters.h"

#include <QStringList>

#include <math.h>

#include <FeatureSet.h>
#include <GreyImage.h>
#include <ImageCache.h>
#include <ImageInfo.h>

using namespace INDI;

DDT::Return EigenFace::staticReturn;

DECLARE_NAMEDORDINAL_INSTANCE(EigenFaceStatus);
DECLARE_NAMEDORDINAL_ID(EigenFaceStatus, Active)
DECLARE_NAMEDORDINAL_ID(EigenFaceStatus, Deleted)
DECLARE_NAMEDORDINAL_ID(EigenFaceStatus, Inconsistent)
DECLARE_NAMEDORDINAL_ID(EigenFaceStatus, Error)

EigenFaceParameters * EigenFace::parameters(const QDir & initDir)
{
    EigenFaceParameters * p = EigenFaceParameters::create(initDir);
    staticReturn = EigenFaceParameters::lastReturn();
    return p;
}
/*
qreal EigenFace::distance(const QLine line)
{
	qreal dx = line.p2().x() - line.p1().x();
	qreal dy = line.p2().y() - line.p1().y();
	return sqrt(dx * dx + dy * dy);
}
*/
qreal EigenFace::pitch(QPointF p1, QPointF p2)
{
    qreal fall = p2.y() - p1.y();
    qreal run  = p2.x() - p1.x();
    if (qIsNull(run))
        return 999999.9;
    else
        return qAbs(fall / run);
}

QImage EigenFace::normalize(const QImage & input, 
                            const Eyes inputEyes,
                            const QSize outSize,
                            const Eyes outEyes,
                            QImage::Format newFormat)
{
    //double width = outSize.width();
    QPointF outCenter = (outEyes.p1() + outEyes.p2()) / 2;
    QPointF eyeCenter = (inputEyes.p1() + inputEyes.p2()) / 2;
    double fall = inputEyes.p2().y() - inputEyes.p1().y();
    double run  = qMax(1, inputEyes.p2().x() - inputEyes.p1().x());
    double theta = atan(fall / run);
    double eyeDistance = sqrt(fall * fall + run * run);
    double outDistance = outEyes.p2().x() - outEyes.p1().x();
    double scale = eyeDistance / outDistance;
    // T (top) point locates line above eyes
    QPointF T = eyeCenter + QPointF(scale * outCenter.y() *  sin(theta),
                                    scale * outCenter.y() * -cos(theta));
    // O (origin) is origin of normalized image
    QPointF O = T - QPointF(scale * outCenter.x() * cos(theta),
                            scale * outCenter.x() * sin(theta));

    if (QImage::Format_Invalid == newFormat)
        newFormat = input.format();
    QImage outImage(outSize, newFormat);
    if (QImage::Format_Indexed8 == newFormat)
    {
        outImage.setNumColors(256);
        outImage.setColorTable(DDT::GreyImage::greyColorTable());
        outImage.fill(128);
    }
    else
        outImage.fill(Qt::gray);

    double scaledSin = scale * sin(theta);
    double scaledCos = scale * cos(theta);
    for (int r = 0; r < outSize.height(); ++r)
    {
        for (int c = 0; c < outSize.width(); ++c)
        {
            double x = O.x() - (double)r * scaledSin + (double)c * scaledCos,
            y = O.y() + (double)r * scaledCos + (double)c * scaledSin;
            int ix = x + 0.5, iy = y + 0.5;
            if (ix >= 0 && ix < input.width()
                && iy >= 0 && iy < input.height())
                {
                if (QImage::Format_Indexed8 == newFormat
                    && QImage::Format_Indexed8 == input.format())
                {
                    int x = input.pixelIndex(ix, iy);
                    outImage.setPixel(c, r, x);
                }
                else if (QImage::Format_Indexed8 == newFormat
                         && QImage::Format_Indexed8 != input.format())
                {
                    QRgb pixel = input.pixel(ix, iy);
                    int x = qGray(pixel);
                    outImage.setPixel(c, r, x);
                }
                else
                {
                    QRgb pixel = input.pixel(ix, iy);
                    outImage.setPixel(c, r, pixel);
                }
            }
        } // for(c)
    } // for(r)

    DDT::FeatureSet fSet;
    DDT::ImageInfo ii;
    fSet.set(DDT::Feature::LeftEye, outEyes.p1());
    fSet.set(DDT::Feature::RightEye, outEyes.p2());
    fSet.calculate();
    ii.addFace(fSet);
    ii.setImageText(&outImage);

    return outImage;
} // normalize()

QImage EigenFace::normalize(const DDT::GreyImage & gi, 
                            const Eyes inputEyes,
                            const QSize outSize,
                            const Eyes outEyes,
                            QImage::Format newFormat)
{
    //double width = outSize.width();
    QPointF outCenter = (outEyes.p1() + outEyes.p2()) / 2;
    QPointF eyeCenter = (inputEyes.p1() + inputEyes.p2()) / 2;
    double fall = inputEyes.p2().y() - inputEyes.p1().y();
    double run  = qMax(1, inputEyes.p2().x() - inputEyes.p1().x());
    double theta = atan(fall / run);
    double eyeDistance = sqrt(fall * fall + run * run);
    double outDistance = outEyes.p2().x() - outEyes.p1().x();
    double scale = eyeDistance / outDistance;
    // T (top) point locates line above eyes
    QPointF T = eyeCenter + QPointF(scale * outCenter.y() *  sin(theta),
                                    scale * outCenter.y() * -cos(theta));
    // O (origin) is origin of normalized image
    QPointF O = T - QPointF(scale * outCenter.x() * cos(theta),
                            scale * outCenter.x() * sin(theta));

    if (QImage::Format_Invalid == newFormat)
        newFormat = QImage::Format_Indexed8;
    QImage outImage(outSize, newFormat);
    if (QImage::Format_Indexed8 == newFormat)
    {
        outImage.setNumColors(256);
        outImage.setColorTable(DDT::GreyImage::greyColorTable());
        outImage.fill(128);
    }
    else
        outImage.fill(Qt::gray);

    double scaledSin = scale * sin(theta);
    double scaledCos = scale * cos(theta);
    for (int r = 0; r < outSize.height(); ++r)
    {
        for (int c = 0; c < outSize.width(); ++c)
        {
            double x = O.x() - (double)r * scaledSin + (double)c * scaledCos,
            y = O.y() + (double)r * scaledCos + (double)c * scaledSin;
            int ix = x + 0.5, iy = y + 0.5;
            if (ix >= 0 && ix < gi.width()
                && iy >= 0 && iy < gi.height())
                {
                if (QImage::Format_Indexed8 == newFormat)
                {
                    int x = gi.pixel(ix, iy);
                    outImage.setPixel(c, r, x);
                }
                else
                {
                    int grey = gi.pixel(ix, iy);
                    QRgb pixel = qRgb(grey, grey, grey);
                    outImage.setPixel(c, r, pixel);
                }
            }
        } // for(c)
    } // for(r)

    DDT::FeatureSet fSet;
    DDT::ImageInfo ii;
    fSet.set(DDT::Feature::LeftEye, outEyes.p1());
    fSet.set(DDT::Feature::RightEye, outEyes.p2());
    fSet.calculate();
    ii.addFace(fSet);
    ii.setImageText(&outImage);

    return outImage;
} // normalize()
