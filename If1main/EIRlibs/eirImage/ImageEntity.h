#ifndef IMAGE_H
#define IMAGE_H
#include "eirImage.h"
#include "../eirTypes/MillisecondTime.h"
#include "../eirCore/VariableSet.h"

#include "ImageClass.h"

#define IMAGE_VARIABLESET(TIVD) \
    TIVD(ImageClass, Image_Class, int, ImageClass::Null) \
    TIVD(QString, Image_Format, QString, QString("jpg")) \
    TIVD(MillisecondTime, Image_GrabTime, qint64, MillisecondTime::null()) \

class EIRIMAGESHARED_EXPORT ImageEntity : public VariableSet
{
public:
    DECLARE_VARIABLESET(IMAGE_VARIABLESET);
    ImageEntity(void);
    ImageEntity(const quint64 key,
                const QString & id,
                const QString & name=QString("ImageEntity"));
    ImageEntity(const VariableSet & other);

    static ImageEntity fromByteArray(const QByteArray & ba,
            const MillisecondTime mst=MillisecondTime::current(),
            const QString FrameId=QString(),
            const ImageClass imageClass=ImageClass::Frame);

    bool isNull(void) const;
    QByteArray binaryData(void) const;
    void setQImage(const QImage & image);
    QImage toQImage(void);

private:
    enum VarIndex
    {
        qimage = 0,
    };
};

#endif // IMAGE_H
