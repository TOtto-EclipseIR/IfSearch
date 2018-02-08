/** @file EigenFaceData.h
*
*	Internal declarations for INDI project EigenFaceGenerator generation
*/

#pragma once
#include <qglobal.h>

#include <EigenFaceBitMask.h>
#include <EigenFaceMaskedArray.h>

#define MAX_LAYERS 512

class QFile;
class QDir;
class QProgressBar;
#include <QMap>
#include <QPoint>
#include <QSize>

#include <Return.h>

// EigenFaceGenerator Data that is fixed for an installation, at least
class  EigenFaceData  : public QObject
{
    Q_OBJECT

public:
    static const QString bitMaskTagName;
    static const QString weightMaskTagName;
    static const QString meanFaceTagName;
    static const QString outputMaskTagName;
    static const QString eigenFaceLayerTagName;

public:
    EigenFaceData(const QString & eigenClass);
    ~EigenFaceData();
    bool isValid(void) const;
    void clear(void);
    Return loadFrom(QFile & xmlFile);
    Return checkRootAttributes(const QDomElement & de);
    void updateProgress(void);

    EigenFaceMaskedArray meanFace(int id=0) const
    { return meanFaces.contains(id) ? meanFaces.value(id) : EigenFaceMaskedArray(); }
    QSize eigenSize(void) const
    { return Mask.size(); }
    int activePixels(void) const
    { return ActivePixels; }
    Eyes eigenEyes(void) const
    { return EigenEyeLine; }
    EigenFaceBitMask mask(void) const
    { return Mask; }
    EigenFaceBitMask outputMask(void) const
    { return OutputMask; }
    EigenFaceMaskedArray weights(void) const
    { return Weights; }
    bool isValidLayer(int ix) const
    { return ix >= 0 && ix < layers.size(); }
    int layerCount(void) const
    { return layers.count(); }
    EigenFaceMaskedArray layer(int ix) const
    { return isValidLayer(ix) ? layers.at(ix) : EigenFaceMaskedArray(); }

    Return create(const QDir & initDir);
    QImage meanFaceImage(int id=0, QImage::Format Format=QImage::Format_Indexed8, bool fullMask=false) const
    { return meanFace(id).toImage(Format, fullMask); }

signals:
    void initProgress(int done, int total);

private:
    QString EigenClass;
    QString Name;
    QString Description;
    int ActivePixels;
    int ActiveLayers;
    QSize EigenSize;
    Eyes EigenEyeLine;
    EigenFaceBitMask Mask;
    EigenFaceBitMask OutputMask;
    EigenFaceMaskedArray Weights;
    QVector<EigenFaceMaskedArray> layers;
    QMap<int, EigenFaceMaskedArray> meanFaces;
}; // EigenFaceData
