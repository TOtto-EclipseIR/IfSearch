#include "AnyColorMatrix.h"

AnyColorMatrix::AnyColorMatrix(void)
{
} // default c'tor()

AnyColorMatrix::AnyColorMatrix(const ColorModel model, const QSize size)
{
    Data.fill(AnyColor(model), size);
} // Model c'tor()

AnyColorMatrix::AnyColorMatrix(const QSize size, const AnyColor fill)
{
    Data.fill(fill, size);
} // Color c'tor()

AnyColorMatrix::AnyColorMatrix(const ColorModel model, QImage image)
{
    Data.fill(AnyColor(model), image.size());
    int h = image.height(), w = image.width();
    image = image.convertToFormat(QImage::Format_RGB32);
    for (int y = 0; y < h; ++y)
    {
        QRgb * inRow = (QRgb *)image.scanLine(y);
        AnyColor * outRow = Data.row(y);
        for (int x = 0; x < w; ++x)
            *outRow++ = AnyColor(*inRow++);
    }
} // Image c'tor()


AnyColorMatrix AnyColorMatrix::operator += (const AnyColorMatrix & other)
{
    if (Data.size() != other.Data.size())
        return AnyColorMatrix();

    int h = Data.height(), w = Data.width();
    for (int y = 0; y < h; ++y)
    {
        const AnyColor * inRow = other.Data.constRow(y);
        AnyColor * outRow = Data.row(y);
        for (int x = 0; x < w; ++x)
            (*outRow++).add(*inRow++);
    }
    return *this;
} // op+=()

QImage AnyColorMatrix::toImage(QImage::Format format) const
{
    QImage image(Data.size(), QImage::Format_RGB32);
    int h = image.height(), w = image.width();
    for (int y = 0; y < h; ++y)
    {
        const AnyColor * inRow = Data.constRow(y);
        QRgb * outRow = (QRgb *)image.scanLine(y);
        for (int x = 0; x < w; ++x)
            *outRow++ = (*inRow++).toQRgb();
    }
    return image.convertToFormat(format);
} // toImage()
