/** @file EigenFaceGenerator.h
*
*	Public declarations for INDI project EigenFaceGenerator template generation
*/

#pragma once
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QImage>
#include <QObject>
class QDir;
class QDomDocument;
class QDomElement;

#include <EigenFaceTemplate.h>
#include <EigenFaceMaskedArray.h>

#include <Eyes.h>
#include <ImageInfo.h>
#include <QQRect.h>
#include <Return.h>

class ImageCache;

class FrontalFaceDetector;
class EyeDetector;
class EigenFaceParameters;	// no user-serviceable parts inside
class EigenFaceData;		// no user-serviceable parts inside

class INDIEF_EXPORT EigenFaceGenerator : public QObject
{
    Q_OBJECT

    // Properties
    Q_PROPERTY(qreal Factor READ factor WRITE setFactor)
    Q_PROPERTY(int EyeScale READ eyeScale WRITE setEyeScale)
    Q_PROPERTY(int RoiScale READ roiScale WRITE setRoiScale)
    Q_PROPERTY(int MinConsistency READ minConsistency WRITE setMinConsistency)
    Q_PROPERTY(QString LeftDetector READ leftDetector WRITE setLeftDetector)
    Q_PROPERTY(QString RightDetector READ rightDetector WRITE setRightDetector)
    Q_PROPERTY(unsigned LeftInterface READ leftInterface WRITE setLeftInterface)
    Q_PROPERTY(unsigned RightInterface READ rightInterface WRITE setRightInterface)

private:
    qreal Factor;
    int EyeScale;
    int RoiScale;
    int MinConsistency;
    QString LeftDetector;
    QString RightDetector;
    unsigned LeftInterface;
    unsigned RightInterface;

public:
    qreal factor(void) const { return Factor; }
    int eyeScale(void) const { return EyeScale; }
    int roiScale(void) const { return RoiScale; }
    int minConsistency(void) const { return MinConsistency; }
    QString leftDetector(void) const { return LeftDetector; }
    QString rightDetector(void) const { return RightDetector; }
    unsigned leftInterface(void) const { return LeftInterface; }
    unsigned rightInterface(void) const { return RightInterface; }
    void setFactor(qreal v) { Factor = v; }
    void setEyeScale(int v) { EyeScale = v; }
    void setRoiScale(int v) { RoiScale = v; }
    void setMinConsistency(int v) { MinConsistency = v; }
    void setLeftDetector(QString v) { LeftDetector = v; }
    void setRightDetector(QString v) { RightDetector = v; }
    void setLeftInterface(unsigned v) { LeftInterface = v; }
    void setRightInterface(unsigned v) { RightInterface = v; }

public:
    EigenFaceGenerator(QObject * parent=0, EigenFaceData * data=0,
                       EigenFaceParameters * parms=0, int instance=0);
    ~EigenFaceGenerator();
    FrontalFaceDetector * setFrontalFaceDetector(void) { return ffd; }
    EyeDetector * leftEyeDetector(void) { return leyed; }
    EyeDetector * rightEyeDetector(void) { return reyed; }
    bool isValid(void) const;

    void clear(void) { clear(ClearAll); }
    Return setImage(const QString & fileName);
    Return setImage(ImageCache * cache, const QString & imageId);
    Return setImage(const QImage & image,
                    const ImageInfo & info=ImageInfo(),
                    const QString & imageSource=QString());
    QImage image(void) const { return originalImage; }
    void setEyePosition(QPoint left, QPoint right)
    { setEyePosition(Eyes(left, right)); }
    void setEyePosition(Eyes eyeLine);
    void setHeadBox(QRect r);

    Eyes normalEyes(void) const;
    Eyes eigenEyes(void) const;
    QImage getReconImage(EigenFaceTemplate * tpl, int vector=0, int layers=0);
    QImage graphVector(EigenFaceTemplate * tpl, int vector=0, QSize size=QSize(), qreal scale=0.0);
    QImage getEigenImage(void);
    QImage getNormalImage(void);
    int numvectors(void) const;
    Return generateTemplate(int minConsistencyOverride=0);

    EigenFaceTemplate templat(void)
    { return tpl; }
    QImage getReconImage(int vector=0, int layers=0)
    { return getReconImage(&tpl, vector, layers); }
    QImage graphVector(int vector=0, QSize size=QSize(), qreal scale=0.0)
    { return graphVector(&tpl, vector, size, scale); }
    QRect headBox(void) const
    { return QQRect(head) * (1.0 / HeadScale); }
    Eyes expectedEyes(void) const
    { return ExpectedEyes * (1.0 / HeadScale); }
    QRect leftEyeRoi(void) const
    { return QQRect(LeftEyeRoi) * (1.0 / HeadScale); }
    QRect rightEyeRoi(void) const
    { return QQRect(RightEyeRoi) * (1.0 / HeadScale); }
    qreal headScale(void) const
    { return HeadScale; }
    Eyes eyes(void) const
    { return origEyes * (1.0 / HeadScale); }
    Eyes adjustedEyes(void) const
    { return adjEyes * (1.0 / HeadScale); }
    int consistency(void) const
    { return Consistency; }
    ImageInfo getInfo(void)
    { ImageInfo ii; fSet.calculate(); ii.addFace(fSet); return ii; }
    QString methodString(void) const
    { return "INDIface"; } // TODO
    QSize expectedEyeSize(void) const
    { return QSize(expectedEyeWidth, expectedEyeWidth); }
    int miniumumEyeWidth(void) const
    { return qRound((qreal)expectedEyeWidth / eyeWidthFactor); }
    int maximumEyeWidth(void) const
    { return qRound((qreal)expectedEyeWidth * eyeWidthFactor); }

    QString	leftEyePerformance(void) const { return LeftEyePerformance; }
    QString	rightEyePerformance(void) const { return RightEyePerformance; }
    int msecGenerate(void) const { return MsecGenerate; }
    int msecScale(void) const { return MsecScale; }
    int msecTemplate(void) const { return MsecTemplate; }

signals:
    void error(QString msg);
    void newImage(int instance);
    void generatedEigenImage(int instance);
    void generatedNormalized(int instance);
    void generatedMasked(int instance);
    void generatedTemplate(int instance);

private:
    enum SignalName
    {
        NewImage, FoundHead, FoundEyes, GeneratedEigenImage,
        GeneratedNormalized, GeneratedMasked, GeneratedTemplate
    };
    enum Clear { ClearAll, ClearImage, ClearHead, ClearEyes, ClearAdjusted };

private:
    void clear(enum Clear what);
    Return read(const QDomElement & de);
    Return adjustEyes(void);
    Return findHead(void);
    Return findEyes(void);
    Return generateNormalized(void);

private:
    EigenFaceTemplate tpl;
    EigenFaceParameters * parms;
    EigenFaceData * data;
    FrontalFaceDetector * ffd;
    EyeDetector * leyed;
    EyeDetector * reyed;
    int instanceId;
    QImage originalImage;
    qreal HeadScale;
    int	Consistency;
    FeatureSet fSet;
    QRect head;
    Eyes origEyes;
    Eyes adjEyes;
    Eyes ExpectedEyes;
    QImage eigenImage;
    QImage normalImage;
    int MsecGenerate;
    int	MsecTemplate;
    int	MsecScale;
    QRect LeftEyeRoi;
    QRect RightEyeRoi;
    int expectedEyeWidth;
    qreal eyeWidthFactor;
    QString LeftEyePerformance;
    QString RightEyePerformance;
    QMap<EigenFaceVectorKey, EigenFaceMaskedArray> differences;
    enum { NotSpecified, AdjustedNeighbors, Neighbors, Other } eyeRoiMethod;
}; // EigenFaceGenerator
