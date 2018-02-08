#include <EigenFaceMaskedArray.h>
#include <EigenFace.h>
#include <EigenFaceData.h>

#include <QColor>
#include <QImage>
#include <QString>
#include <QVector>

#include <GreyImage.h>
#include <Return.h>

#include <math.h>



QImage EigenFaceMaskedArray::toImage(QImage::Format Format, bool fullMask) const
{
    if ( ! isValid())
        return QImage();

    QSize sz(efData->mask().size());
    QImage image(sz, QImage::Format_Indexed8);
    image.setNumColors(256);
    image.setColorTable(GreyImage::greyColorTable());
    image.fill(128);

    int x = 0;
    for (int row = 0; row < sz.width() ; ++row)
        for (int col = 0; col < sz.height(); ++col)
            if (efData->mask().at(col, row))
            {
        if (fullMask || efData->outputMask().at(col, row))
        {
            qreal f = data.at(x);
            if (f < 0.0)
                image.setPixel(col, row, (int)128 - (128.0 * f / minData));
            else
                image.setPixel(col, row, (int)127 + (128.0 * f / maxData));
        }
        ++x;
    }
    image = image.convertToFormat(Format);

    return image;
} // toImage()


Return EigenFaceMaskedArray::fromImage(const QImage & image)
{
    if ( ! efData || ! efData->isValid())
        return Return(EigenFace::ReturnInvalidStructure, "FaceData");
    if ( ! efData->mask().isValid())
        return Return(EigenFace::ReturnInvalidStructure, "BitMask");
    if ( ! efData->weights().isValid())
        return Return(EigenFace::ReturnInvalidStructure, "WeightMask");

    int rows = efData->eigenSize().height();
    int cols = efData->eigenSize().width();

    clear();
    data.reserve(efData->activePixels());
    qreal totalGrey = 0.0;

    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (efData->mask().at(x,y))
            {
        qreal grey;
        if (QImage::Format_Indexed8 == image.format())
        {
            int pixel = image.pixel(x, y);
            grey = (qreal)pixel / 255.0;
        }
        else
        {
            QRgb pixel = image.pixel(x, y);
            // accumulateColor(pixel);
            grey = (qreal)qGray(pixel) / 255.0;
        }
        grey *= efData->weights().at(data.size());
        totalGrey += grey;
        if (grey < minData)	minData = grey;
        if (grey > maxData)	maxData = grey;
        data.append(grey);
    } // if, for, for

    if ( ! isValid())
        return Return(EigenFace::ReturnInvalidStructure, "Invalid Count");

    qreal averageGrey = totalGrey / (qreal)data.size();
    qreal sumSquares = subtract(averageGrey);

    qreal variance = sqrt(sumSquares);
    divideBy(variance);
    // IEIGENFACE_PRELIMINARY
    /*
	QImage varImage = toImage(QImage::Format_Indexed8, false);
	varImage.save("varImage.jpg");
*/
    return Return();
} // fromImage()

Return EigenFaceMaskedArray::generateEigenImage(const QImage & image, Eyes eyes)
{
    if (image.isNull())
        return Return(EigenFace::ReturnNullImage, "InputImage");
    if ( ! efData)
        return Return(EigenFace::ReturnNoData);
    Eyes eigenEyes = efData->eigenEyes();
    if (eigenEyes.isNull())
        return Return(EigenFace::ReturnInvalidStructure, "eigenEyes");
    QSize eigenSize = efData->eigenSize();
    if ( ! eigenSize.isValid())
        return Return(EigenFace::ReturnInvalidStructure, "eigenSize");
    if (eyes.isNull())
        return Return(EigenFace::ReturnNoEyes);
    eigenImage = EigenFace::normalize(image, eyes, eigenSize, eigenEyes);
    if (eigenImage.isNull())
        return Return(EigenFace::ReturnNullImage, "EigenFace");
    return Return();
} // generateEigenImage()

Return EigenFaceMaskedArray::generateEigenImage(const GreyImage & gi, Eyes eyes)
{
    if (gi.isNull())
        return Return(EigenFace::ReturnNullImage, "InputImage");
    if ( ! efData)
        return Return(EigenFace::ReturnNoData);
    Eyes eigenEyes = efData->eigenEyes();
    if (eigenEyes.isNull())
        return Return(EigenFace::ReturnInvalidStructure, "eigenEyes");
    QSize eigenSize = efData->eigenSize();
    if ( ! eigenSize.isValid())
        return Return(EigenFace::ReturnInvalidStructure, "eigenSize");
    if (eyes.isNull())
        return Return(EigenFace::ReturnNoEyes);
    eigenImage = EigenFace::normalize(gi, eyes, eigenSize, eigenEyes);
    if (eigenImage.isNull())
        return Return(EigenFace::ReturnNullImage, "EigenFace");
    return Return();
} // generateEigenImage()

Return EigenFaceMaskedArray::generate(const QImage & image,
                                           const Eyes eyes,
                                           const EigenFaceVectorKey vecKey)
{
    Return rtn;

    if ( ! efData)
        return Return(EigenFace::ReturnNoData);
    if ( ! efData->meanFace(vecKey.meanId()).isValid())
        return Return(EigenFace::ReturnInvalidStructure, "MeanFace");

    rtn = generateEigenImage(image, eyes);
    if (rtn.isError())
        return rtn;

    rtn = fromImage(eigenImage);
    if (rtn.isError())
        return rtn;

    Residual = subtract(efData->meanFace(vecKey.meanId()));
    setVectorKey(vecKey);
    return rtn;
} // generate()


Return EigenFaceMaskedArray::generate(const GreyImage & gi,
                                           const Eyes eyes,
                                           const EigenFaceVectorKey vecKey)
{
    Return rtn;

    if ( ! efData)
        return Return(EigenFace::ReturnNoData);
    if ( ! efData->meanFace(vecKey.meanId()).isValid())
        return Return(EigenFace::ReturnInvalidStructure, "MeanFace");

    rtn = generateEigenImage(gi, eyes);
    if (rtn.isError())
        return rtn;

    rtn = fromImage(eigenImage);
    if (rtn.isError())
        return rtn;

    Residual = subtract(efData->meanFace(vecKey.meanId()));
    setVectorKey(vecKey);
    return rtn;
} // generate()
