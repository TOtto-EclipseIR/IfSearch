/** @file Detector.h
*
*	Public declarations for INDI project object detectors
*/

#pragma once
#include <qglobal.h>
#ifdef INDIFFD_LIB
# define INDIFFD_EXPORT Q_DECL_EXPORT
#else
# define INDIFFD_EXPORT Q_DECL_IMPORT
#endif

#include <QDomElement>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QMultiMap>
#include <QObject>
#include <QSize>
#include <QString>
#include <QStringList>
class QSettings;
class QDomDocument;

#ifdef USE_OCV2
#include <cv.h>
#else
//#include <objdetect/include/opencv2/objdetect/objdetect.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#endif
#include "VersionInfo.h"

class ImageCache;
#include <QQRect.h>
#include <Return.h>

class INDIFFD_EXPORT DetectorResult
{
private:
    friend class HaarDetector;
    DetectorResult(QRect Rect)
        : k(0), sco(0), rect(Rect)
        , ptAverage(Rect.topLeft()), szAverage(Rect.size())
    { allRects.append(Rect); }

public:
    DetectorResult(int Rank=0, int Score=0, QRect Rect=QRect(),
                   QList<QRect> AllRects=QList<QRect>())
        : k(Rank), sco(Score), rect(Rect), allRects(AllRects) {}

public:
    int rank(void) { return k; }
    int score(void) const { return sco; }
    QRect rectangle(void) const { return rect; }
    QList<QRect> allRectangles(void) { return allRects; }

private:
    void addRect(QRect Rect) { rect = rect.united(Rect); allRects.append(Rect); }
    void addToAverage(QRect Rect);

private:
    int k;
    int sco;
    QRect rect;
    QPointF ptAverage;
    QSizeF szAverage;
    QList<QRect> allRects;
};

/** HaarDetector - Base class for OpenCV Haar detectors
    *
    */
class INDIFFD_EXPORT HaarDetector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ClassName READ className)
    Q_PROPERTY(qreal ClassFactor READ classFactor)
    Q_PROPERTY(QString DetectorName READ detectorName)
    Q_PROPERTY(QString DetectorDescription READ detectorDescription)
    Q_PROPERTY(QSize DetectorSize READ detectorSize)
    Q_PROPERTY(int MaxResults READ maxResults WRITE setMaxResults)
    Q_PROPERTY(int MinQuality READ minQuality WRITE setMinQuality)
    Q_PROPERTY(bool ForceFind READ forceFind WRITE setForceFind)
    Q_PROPERTY(qreal Factor READ factor WRITE setFactor)
    Q_PROPERTY(qreal MaxDensity READ maxDensity WRITE setMaxDensity)
    Q_PROPERTY(int MaxDetectors READ maxDetectors WRITE setMaxDetectors)
    Q_PROPERTY(qreal Scale READ scale WRITE setScale)
    Q_PROPERTY(int Flags READ flags WRITE setFlags)
    Q_PROPERTY(int Adjust READ adjust WRITE setAdjust)
    Q_PROPERTY(int MaxAcross READ maxAcross WRITE setMaxAcross)
    Q_PROPERTY(int MinAcross READ minAcross WRITE setMinAcross)
    Q_PROPERTY(int MaxDimension READ maxDimension WRITE setMaxDimension)
    Q_PROPERTY(int MaxPixels READ maxPixels WRITE setMaxPixels)
    Q_PROPERTY(int MinPixels READ minPixels WRITE setMinPixels)
    Q_PROPERTY(int GroupMethod READ groupMethod WRITE setGroupMethod)
    Q_PROPERTY(int GroupThreshold READ groupThreshold WRITE setGroupThreshold)
    Q_PROPERTY(int OverlapThreshold READ overlapThreshold WRITE setOverlapThreshold)
    Q_PROPERTY(int NeighborThreshold READ neighborThreshold WRITE setNeighborThreshold)
    Q_PROPERTY(QString MarkExtraColor READ markExtraColor WRITE setMarkExtraColor)
    Q_PROPERTY(QString MarkScaleColor READ markScaleColor WRITE setMarkScaleColor)
    Q_PROPERTY(QString MarkColors READ markColors WRITE setMarkColors)
    Q_PROPERTY(bool MarkAll READ markAll WRITE setMarkAll)

public: // Property accessors
    bool markAll(void) { return MarkAll; }
    void setMarkAll(bool v) { MarkAll = v; }
    QString markExtraColor(void) { return MarkExtraColor; }
    void setMarkExtraColor(QString v) { MarkExtraColor = v; }
    QString markScaleColor(void) { return MarkScaleColor; }
    void setMarkScaleColor(QString v) { MarkScaleColor = v; }
    QString markColors(void) { return MarkColors; }
    void setMarkColors(QString v) { MarkColors = v; }
    QString MarkExtraColor;
    QString MarkScaleColor;
    QString MarkColors;
    bool MarkAll;

    QString className(void) { return ClassName; }
    qreal classFactor(void) { return ClassFactor; }
    QString detectorName(void) { return DetectorName; }
    QString detectorDescription(void) { return DetectorDescription; }
    QSize detectorSize(void) { return DetectorSize; }
    int maxResults(void) { return MaxResults; }
    int minQuality(void) { return MinQuality; }
    bool forceFind(void) { return ForceFind; }
    qreal maxDensity(void) { return MaxDensity; }
    int maxDetectors(void) { return MaxDetectors; }
    qreal factor(void) { return Factor; }
    int adjust(void) { return Adjust; }
    qreal scale(void) { return Scale; }
    int flags(void) { return Flags; }
    int maxAcross(void) { return MaxAcross; }
    int minAcross(void) { return MinAcross; }
    int maxDimension(void) { return MaxDimension; }
    int maxPixels(void) { return MaxPixels; }
    int minPixels(void) { return MinPixels; }
    int groupMethod(void) { return GroupMethod; }
    int groupThreshold(void) { return GroupThreshold; }
    int overlapThreshold(void) { return OverlapThreshold; }
    int neighborThreshold(void) { return NeighborThreshold; }
    void setMaxResults(int v) { MaxResults = v; }
    void setMinQuality(int v) { MinQuality = v; }
    void setForceFind(bool v) { ForceFind = v; }
    void setMaxDensity(qreal v) { MaxDensity = v; }
    void setMaxDetectors(int v) { MaxDetectors = v; }
    void setFactor(qreal v) { Factor = v; }
    void setAdjust(int v) { Adjust = v; }
    void setScale(qreal v) { Scale = v; }
    void setFlags(int v) { Flags = v; }
    void setMaxAcross(int v) { MaxAcross = v; }
    void setMinAcross(int v) { MinAcross = v; }
    void setMaxPixels(int v) { MaxPixels = v; }
    void setMaxDimension(int v) { MaxDimension = v; }
    void setMinPixels(int v) { MinPixels = v; }
    void setGroupMethod(int v) { GroupMethod = v; }
    void setGroupThreshold(int v) { GroupThreshold = v; }
    void setOverlapThreshold(int v) { OverlapThreshold = v; }
    void setNeighborThreshold(int v) { NeighborThreshold = v; }

protected:	// Properties
    int MaxResults;
    int MinQuality;
    bool ForceFind;
    qreal MaxDensity;
    int MaxDetectors;
    qreal Factor;
    int Adjust;
    qreal Scale;
    int Flags;
    int MaxAcross;
    int MinAcross;
    int MaxDimension;
    int MaxPixels;
    int MinPixels;
    int GroupMethod;
    int GroupThreshold;
    int OverlapThreshold;
    int NeighborThreshold;
    //		int AreaThreshold;

public:
    enum Interface
    {
        Default = 0,
        C, /* 1 */
        CascadeClassifier, /* 2 */
        // future: HOGDescriptor = 3, ?
        nInterface
    };
    enum
    {
        GroupByCenters = 1, GroupByOverlap, GroupByNeighbors,
        GroupInternal, GroupInternalAllObjects
    };
    int DefaultGroupMethod;
    void setDefaultGroupMethod(int v) { DefaultGroupMethod = v; }
    QImage detectImage(void) const;

protected:
    HaarDetector(const QString & classname,
                 const qreal classfactor,
                 const Interface iface,
                 QObject * parent=0);
    QDomElement classDetectors(const QDomDocument & doc);
    virtual int scaleScore(qreal raw)
    { return qBound(0, qRound(DETECTOR_SCALE * raw), 999); }
    qreal minAspect(void) const;
    qreal maxAspect(void) const;
    void groupObjects(bool returnAll=false);
    CvHaarClassifierCascade * cascade(void)
    { return pCascade; }
    qreal scaleFactor(void) const;
    int groupFactor(void) const;
    QList<QColor> markColorList(void) const;

protected slots:
    bool process(bool returnAll=false);

public:
    ~HaarDetector();
    bool isValid(void) const { return true; } // TODO
    static VersionInfo cvVersion(void);
    void selectDetector(const QString & name);
    void selectXml(const QString & fileName);
    void setCache(ImageCache * Cache) { cache = Cache; }
    void clear(void);
    void setImage(QImage img);
    void setImage(const QString & ImageId);
    QImage image(void) const;
    int msec(void) const { return msecProcess; };
    int strange(void) const { return strangeObjects; }
    int raw(void) const { return rawObjects; }
    QString performanceString(void) const;
    QList<DetectorResult> getResults(void) const { return results; }
    QList<QRect> getAllObjects(void) const { return allObjects; }
    QList<QSize> detectorSizes(void) const;
    bool hasDetector(void);
    QString methodString(void);
    QSize sizeFromXml(const QString & fileName);
    QSize minObjectSize(void) const;
    QSize maxObjectSize(void) const;

signals:
    void error(QString message);

public:
    static Return initialize(QFile * xmlFile);
    QStringList detectorsAvailable(void);
    QString detectorsDefault(void);
    QString detectorDescription(const QString & DetectorName);
    QSize detectorSize(const QString & DetectorName);
    bool isStrange(QQRect rect);

private:
    bool processC(bool returnAll=false);
    bool processCascadeClassifier(bool returnAll=false);
    bool loadXmlCascade(const QString & xmlFilename);
    void groupByCenters(bool returnAll=false);
    void groupByOverlap(bool returnAll=false);
    void groupByNeighbors(bool returnAll=false);
    void groupInternal(bool returnAll=false);
    void handleResults(bool returnAll=false);
    virtual QQRect doAdjust(QQRect r) = 0;

protected:
    QList<QRect> allObjects;
    QList<DetectorResult>  results;
    QMultiMap<double, DetectorResult> allResults;
    Interface interface_;
    CvHaarClassifierCascade * pCascade;

private:
    static QDomDocument docDetectors;
    static QDir dirDetectors;
    QDomElement deDetectors;
    QSettings * settings;
    QString ClassName;
    qreal ClassFactor;
    QString DetectorName;
    QString DetectorDescription;
    QSize DetectorSize;
    QString DetectorFileName;
#ifndef USE_OCV2
    cv::CascadeClassifier cvCascade_;
    cv::Mat cvMat_Grey;
#endif
//    IplImage * ipl_Grey;
    QImage imgOrig;
    int origScale;
    QSize scaled_size;
    ImageCache * cache;
    int msecProcess;
    int strangeObjects;
    int rawObjects;
}; // class HaarDetector

class INDIFFD_EXPORT FrontalFaceDetector : public HaarDetector
{
    Q_OBJECT

public:
    FrontalFaceDetector(const unsigned interface=0, QObject *parent = 0);
    ~FrontalFaceDetector();
    bool process(QList<DetectorResult> * resultList=0, bool returnAll=false);
    virtual int scaleScore(qreal raw) { return HaarDetector::scaleScore(raw); }
    virtual QQRect doAdjust(QQRect r);

signals:
    void processed(void);
}; // class FrontalFaceDetector

class INDIFFD_EXPORT EyeDetector : public HaarDetector
{
    Q_OBJECT

public:
    EyeDetector(const unsigned interface=0, QObject *parent = 0);
    ~EyeDetector();
    bool process(QList<DetectorResult> * resultList=0, bool returnAll=false);
    virtual int scaleScore(qreal raw) { return HaarDetector::scaleScore(raw); }
    virtual QQRect doAdjust(QQRect r) { return r; }

signals:
    void processed(void);
}; // class EyeDetector
