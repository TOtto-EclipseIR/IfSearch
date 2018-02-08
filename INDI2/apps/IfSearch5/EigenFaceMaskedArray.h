/** @file EigenFaceMaskedArray.h
*
*	Internal declarations for INDI project EigenFaceGenerator generation
*/

#pragma once
#include <qglobal.h>

#include <EigenFaceVector.h>

#include <QImage>
class QDomDocument;

#include <Eyes.h>
#include <NamedEnum.h>
    class GreyImage;

    class EigenFaceData;

    class  EigenFaceMaskedArray
    {
    public:
        enum { StreamId=0x45464D41 };

    public:
        EigenFaceMaskedArray(const EigenFaceData * data=0, int id=0);
        EigenFaceMaskedArray(const EigenFaceData * data, const QByteArray & ba);
        ~EigenFaceMaskedArray();
        // IEIGENFACE_PRELIMINARY
        //		EigenFaceMaskedArray(EigenFaceData * EFdata, int id, int length, float * fp);

        void clear(void);
        bool isValid(void) const;
        bool isNull(void) const
        { return minData > maxData; }
        bool isUniform(void)
        { return qIsNull(maxData - minData); }
        int id(void) const
        { return Id; }
        int count(void) const
        { return data.count(); }
        qreal at(int x) const
        { return (x >= 0 && x < data.size()) ? data.at(x) : 0.0; };
        bool isValid(int x) const;
        void fill(qreal v);
        void setResidual(qreal v)
        { Residual = v; }
        int consistency(void) const
        { return qIsNull(Residual) ? 0 : (int)(1000.0 / (1.0 + Residual)); }
        QImage image(void) const
        { return eigenImage; }
        QImage toImage(QImage::Format Format=QImage::Format_Indexed8, bool fullMask=false) const;
        QPixmap toPixmap(void) const;
        Return read(const QDomElement & de);
        Return read(const QByteArray & ba);
        Return read(QIODevice * io);
        Return readXmlFile(const QString & fileName, const QString & tagName);
        Return write(QDomElement * de) const;
        Return write(QDomDocument * doc, const QString & tagName) const;
        Return write(QByteArray * ba) const;
        Return write(QIODevice * io) const;
        EigenFaceVectorKey vectorKey(void) { return VectorKey; }
        void setVectorKey(EigenFaceVectorKey id) { VectorKey = id; };
        EigenFaceVector generateVector(int maxLayers=0);
        Return reconstruct(const EigenFaceVector & vector, int numCoefficients=0);

        Return generate(const QImage & image,
                             const Eyes eyes,
                             const EigenFaceVectorKey vecId=EigenFaceVectorKey());
        Return generate(const GreyImage & gi,
                             const Eyes eyes,
                             const EigenFaceVectorKey vecId=EigenFaceVectorKey());

        Return generateEigenImage(const QImage & image, const Eyes eyes);
        Return generateEigenImage(const GreyImage & gi, const Eyes eyes);

        // create from visible image with weight mask
        Return fromImage(const QImage & image);
	
        // subtract other from this, return sum of square of result
        qreal subtract(const EigenFaceMaskedArray & other);

        // subtract constant from data, return sum of square of result
        qreal subtract(const qreal subtrahend);

        // calculate difference between other and this,
        //	return sum of product of difference and other
        qreal difference(const EigenFaceMaskedArray & other) const;

        // divide each value by divisor; update min and max values
        void divideBy(const qreal divisor);

        // return dot product between two arrays
        qreal dotProduct(const EigenFaceMaskedArray & other) const;

        // add corresponding elements from other multiplied by coefficient
        void add(const EigenFaceMaskedArray & other, qreal coefficient);


    private:
        const EigenFaceData * efData;
        int Id;
        EigenFaceVectorKey	VectorKey;
        qreal Residual;
        qreal minData, maxData;
        QImage eigenImage;
        QVector<qreal>	data;

    }; // EigenFaceMaskedArray

