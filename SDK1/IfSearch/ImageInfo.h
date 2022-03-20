/** @file ImageInfo.h
*	Public declaration of ImageInfo class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once

#include <qglobal.h>

class QDomDocument;
#include <QList>
#include <QObject>

#include <AttributeSet.h>
#include <FeatureSet.h>

class ImageInfo
{
public:
    ImageInfo(void);
    ImageInfo(const QImage & image);
    ~ImageInfo();
    ImageInfo & operator=(const ImageInfo & that);
    void addFace(const FeatureSet & fSet);
    FeatureSet face(int x) const
    { return (x >=0 && x < fSets.size()) ? fSets.at(x) : FeatureSet(); }
    bool writeDomElement(QDomElement * de) const;
    bool read(const QDomElement & de);
    bool setImageText(QImage * image, QString tag="INDIinfo");
    bool isEmpty(void);
    //		void clear(void) { aSet.clear(), fSets.clear(); }
    void clear(void) { fSets.clear(); }
    void clearFaces(void) { fSets.clear(); }
    QList<QPoint> eyes(void) const;
    QList<QRect> heads(void) const;
    QList<QRect> heads(QString usage) const;

private:
    AttributeSet	aSet;
    QList<FeatureSet> fSets;
}; // class ImageCache
