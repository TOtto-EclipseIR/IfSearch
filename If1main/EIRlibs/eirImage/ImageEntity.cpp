#include "ImageEntity.h"

#include <QtGui/QImage>

#include "../eirExe/Log.h"

ImageEntity::ImageEntity(void)
    : VariableSet("ImageEntity")
{
    FUNCTION();
    DEFINE_VARIABLESET_CTORS(IMAGE_VARIABLESET);
}

ImageEntity::ImageEntity(const quint64 key,
            const QString & id,
            const QString & name)
    : VariableSet(key, id, name)
{
    FUNCTION(key, id, name);
    DEFINE_VARIABLESET_CTORS(IMAGE_VARIABLESET);
}

ImageEntity::ImageEntity(const VariableSet & other)
    : VariableSet(other)
{
    FUNCTION(other.name());
}

ImageEntity ImageEntity::fromByteArray(const QByteArray & ba,
                                       const MillisecondTime mst,
                                       const QString FrameId,
                                       const ImageClass imageClass)
{
    FUNCTION(ba.size(), mst.toString(), FrameId, imageClass.name());

    ImageEntity ie(0, FrameId.isEmpty()
                           ? mst.toString("DyyyyMMdd-Thhmmsszzz")
                           : FrameId);
    ie.set(ba);
    ie.setImage_Class(imageClass);
    ie.setImage_GrabTime(mst);
    FNRETURN(ie.value().size());
    return ie;
}

bool ImageEntity::isNull(void) const
{
    return ImageClass::Null == getImage_Class();
}

QByteArray ImageEntity::binaryData(void) const
{
    return VariableSet::value();
}

void ImageEntity::setQImage(const QImage & image)
{
    FUNCTION(image.size());
    VariableSet::set(qimage, image);
}

QImage ImageEntity::toQImage(void)
{
    FUNCTION();
    QImage qImage = VariableSet::value(qimage).value<QImage>();
    if (qImage.isNull())
    {
        QByteArray ba(binaryData());
        if (ba.isEmpty())
        {
            WARNING("No binary data to make QImage");
        }
        else
        {
            qImage = QImage::fromData(ba, qPrintable(getImage_Format()));
            if (qImage.isNull())
                qImage = QImage::fromData(ba);
            if (qImage.isNull())
            {
                WARNING("QImage::loadFromData() failed; %1 bytes", ba.size());
                DUMPHEX(ba.constData(), 256)
            }
            else
            {
                setQImage(qImage);
            }
        }
    }
    FNRETURN(qImage.byteCount());
    return qImage;
}
