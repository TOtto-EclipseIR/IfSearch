#ifndef IFSEARCH_H
#define IFSEARCH_H

#ifndef QT_NO_DEBUG
//#define ENABLE_AVGFACE
#endif

#include <QApplication>
#include <QStringList>

#include <Detector.h>
#include <EigenFaceTemplate.h>
#include <EigenFaceSearchSettings.h>
#include <ImageCache.h>
class QFileSystemWatcher;

#include <Eyes.h>
#include <TransformProperties.h>

#include "../../INDI2/EIRlibs/eirTypes/MillisecondTime.h"
#include "../../INDI2/EIRlibs/eirCore/IdGenerator.h"
#include "../../INDI2/EIRlibs/eirFile/CsvWriter.h"
#include "../../INDI2/EIRlibs/eirIb2/FramePerformance.h"
#include "../../INDI2/EIRlibs/eirIb2/FrameStatistics.h"

namespace DDT
{
    class FileWriteProfile;
    class FileWriter;
    class ImageSource;
    class InfoOutputSetting;
    class InputHotdir;
    class Settings;
    class Setting;
}

namespace INDI
{
    class EigenFaceData;
    class EigenFaceGenerator;
    class EigenFaceParameters;
    class EyeDetector;
    class FrontalFaceDetector;
    class FaceBase;
}

class AverageFace;
class ClothesMatchProperties;
class ClothesMatcher;
class ColorCorrectProperties;
class ColorCorrection;
class EigenFaceSearcher;
class EigenFaceSearchResultList;
class HeightGrid;
class Resolver;
class SkinDetector;
class SkinMatchProperties;
class SkinMatcher;
class FSDirectBridge;
#ifdef ENABLE_WATCHDOG
class WatchDog;
#endif

class IfSearch : public QApplication
{
    Q_OBJECT

public:
    IfSearch(int argc, char *argv[]);
    ~IfSearch();

#ifdef ENABLE_WATCHDOG
signals:
    void petDog(QVariant value);
private slots:
    void dogBite(QVariant lastValue,
                 MillisecondTime lastTime);
#endif

private:
    DDT::Return initEigenFace(void);
    DDT::Return writeOutputImage(QPair<QString,INDI::DetectorResult> face,
                                 int consistency,
                                 QImage normImage,
                                 const EigenFaceSearchResultList & resList);
    DDT::Return writeXmlResult(QPair<QString,INDI::DetectorResult> face,
                               const EigenFaceSearchResultList & resList);
    DDT::Return writeMatches(const EigenFaceSearchResultList & resList);

    void enrollError(const QString & reason);
    void searchError(const QString & reason);
    void retrieveError(const QString & reason);
    void retrieveNotFound(const QString & reason);
    bool check(const quint64 daysExpire) const;

#ifdef ENABLE_AVGFACE
    bool avgFaceInit(void);
    bool avgFaceProcess(QImage normImage, const Eyes normEyes);
    bool avgFaceFinish(void);
#endif

private slots:
    void init(void);
    void start(void);
    void run(void);
    void pulse(void);
    void hotdirReady(void);
    void hotdirEmpty(void);
    void grabbed(QString ImageId);
    void changed(const QString & key);
    void processGrab(void);
    void processFace(void);
    void doEnroll(void);
    void doDelete(void);
    void doRemove(void);
    void doRemoveAll(void);
    void doRename(void);
    void doReload(void);
    void doManual(void);
    void doReenroll(void);
    void enrollScanDir(void);
    void enrollGrab(void);
    void enrollFinish(void);
    void doSearch(void);
    void doVerify(void);
    void doAuthenticate(void);
    void doRetrieve(void);
    void doRetrieveAll(void);
    void doStatus(void);
    void doPersonReport(void);
    void searchScanDir(void);
    void searchGrab(void);
    void searchOrVerify(void);
    void searchFinish(void);
    void authFinished(void);
    void tryAuthenticate(void);
    void authDirChange(QString path);
    void retrievePerson(void);
    void retrieveFaceOnly(void);
    void retrieveFinish(void);
    void directorySet(QString profileName, QString absoluteDirName);
    void fileWritten(QString absoluteFilePath);
    void done(void);
    void warn(QString msg);
    void error(QString msg);
    void error2(QString id, QString msg);

private:
    VersionInfo version;
    DDT::Settings * appSettings;
    DDT::InfoOutputSetting * infoSetting;
    DDT::Setting * optNoPrompt;
    DDT::Setting * optInput;
    DDT::Setting * optPause;
    DDT::Setting * optRestartSecs;
    DDT::Setting * optLogFile;
    DDT::Setting * optLogDetail;
    DDT::Setting * optDetectEnable;
    DDT::Setting * optMatchEnable;
    DDT::Setting * optGenerateEnable;
    DDT::Setting * optGenerateFactor;
    DDT::Setting * optFaceColorEnable;
    DDT::Setting * optHeightEnable;
    DDT::Setting * optClothesEnable;
    DDT::Setting * optResolveEnable;
    DDT::Setting * optMarkAllColor;
    DDT::Setting * optMarkEyes;
    DDT::Setting * optMarkAllEyeColor;
    DDT::Setting * optMarkFaceColor;
    DDT::Setting * optMarkEyeRoiColor;
    DDT::Setting * optMarkBadFaceColor;
    DDT::Setting * optMarkNoEyesColor;
    DDT::Setting * optMarkEyeColor;
    DDT::Setting * optMarkBackgroundColor;
    DDT::Setting * optMarkBackgroundTransparency;
    DDT::Setting * optMarkOverCrop;
    DDT::Setting * optMarkOverMark;
    DDT::Setting * optMarkBackgroundFile;
    DDT::Setting * optMarkClothes;
    DDT::Setting * optAppendPersonId;
    DDT::Setting * optWriteFaceInfo;
    DDT::Setting * optEnrollCommand;
    DDT::Setting * optSearchCommand;
    DDT::Setting * optRetrieveCommand;
    DDT::Setting * optInputOverCrop;
    DDT::Setting * optForceMarked;
    DDT::Setting * optShutdown;
    DDT::Setting * optResolveMin;
    DDT::Setting * optResolveMax;
    DDT::Setting * optSourceChanged;
#ifdef ENABLE_WATCHDOG
    DDT::Setting * optWatchDogValue;
#endif
#ifdef ENABLE_AVGFACE
    DDT::Setting * optAvgFaceEnable;
    DDT::Setting * optAvgFaceMinConsistency;
    DDT::FileWriteProfile * fwpAvgFace;
#endif

    QString eigenFaceDataDir;
//    QString eigenFaceParmDir;
    QString faceBaseBaseDir;
    QString detectorsXml;
    int faceBaseMaxLoad;   
#ifdef ENABLE_RELOADMINS
    MillisecondDelta faceBaseReload;
#endif

    DDT::ImageSource * camera;

    MillisecondTime streamBase_mst;
    bool paused;
    bool pausePending;
    bool ffdBusy;
    AverageFace * avgFace;
    INDI::FrontalFaceDetector * ffd;
    INDI::EigenFaceGenerator * eigenFace;
    INDI::EigenFaceData * eigenData;
    INDI::EigenFaceParameters * eigenParms;
    EigenFaceSearcher * eigenMatcher;
    INDI::EigenFaceSearchSettings matchSettings;
    INDI::EigenFaceSearchSettings searchSettings;
    INDI::FaceBase * faceBase;
    DDT::InputHotdir * hotdir;
    DDT::ImageCache imageCache;
    //QString searchId;
    QImage markedImage;
    QImage grabImage;
    bool isMarked;
    int FramesProcessed;
    int NullFrames;
    int FacesProcessed;
    int numSearches;
    int msecSearches;
    QString commandMode;

    IdGenerator idGenerator;
    DDT::FileWriter * writer;
    DDT::FileWriteProfile * fwpCapture;
    DDT::FileWriteProfile * fwpCapture2;
    DDT::FileWriteProfile * fwpMarked;
    DDT::FileWriteProfile * fwpNoMark;
    DDT::FileWriteProfile * fwpUnMarked;
    DDT::FileWriteProfile * fwpDetect;
    DDT::FileWriteProfile * fwpGenerate;
    DDT::FileWriteProfile * fwpSkin;
    DDT::FileWriteProfile * fwpCharcol;
    DDT::FileWriteProfile * fwpMarkedFace;
    DDT::FileWriteProfile * fwpRecon;
    DDT::FileWriteProfile * fwpVector;
    DDT::FileWriteProfile * fwpNoFace;
    DDT::FileWriteProfile * fwpNoEyes;
    DDT::FileWriteProfile * fwpBadFace;
    DDT::FileWriteProfile * fwpNoMatch;
    DDT::FileWriteProfile * fwpMatch;
    DDT::FileWriteProfile * fwpFace;
    DDT::FileWriteProfile * fwpBody;
    DDT::FileWriteProfile * fwpClothes;
    DDT::FileWriteProfile * fwpNoClothes;
    DDT::FileWriteProfile * fwpClothesFace;
    DDT::FileWriteProfile * fwpHeight;
    DDT::FileWriteProfile * fwpNoHeight;
    DDT::FileWriteProfile * fwpHeightFace;
    DDT::FileWriteProfile * fwpFaceCache;
    DDT::FileWriteProfile * fwpXml;
    DDT::FileWriteProfile * fwpImage;
    DDT::FileWriteProfile * fwpRetrieve;
    DDT::FileWriteProfile * fwpRetrRecon;
    DDT::FileWriteProfile * fwpSearch;
    DDT::FileWriteProfile * fwpResolveFace;
    DDT::FileWriteProfile * fwpResolveMarked;
    DDT::FileWriteProfile * fwpNoFaceColor;
    QMap<QString,DDT::FileWriteProfile *> fwpsFaceColor;

    QList<QPair<QString,INDI::DetectorResult> > pendingFaces;
    QString resultsDelimiter;

#ifdef ENABLE_WATCHDOG
    WatchDog * _watchdog;
#endif
    int enrollTimeout;
    bool enrollPersonMode;
    QDir enrollDir;
    int enrollPersonKey;
    QString enrollPersonId;
    QStringList enrollPending;
    QString enrollResults;

    int searchTimeout;
    QDir searchDir;
    int searchPersonKey;
    QString searchPersonId;
    INDI::EigenFaceTemplate searchTemplate;
    QStringList searchPending;
    QString searchResults;
    int authConfidence;
    int authUpdate;
    QFileSystemWatcher * authWatcher;

    int retrievePersonKey;
    QString retrievePersonId;
    int retrieveFaceKey;
    QString retrieveFaceId;
    QString retrieveResults;
    ClothesMatchProperties * clothesMatchProperties;
    ClothesMatcher * clothesMatcher;
    SkinMatcher * skinMatcher;
    SkinDetector * skinDetector;
    Resolver * resolver;
    HeightGrid * heightGrid;
    TransformProperties xformProps;
    FrameStatistics * frameStatistics;
    FramePerformance * framePerformance;
    CsvWriter * fpWriter;
//    ColorCorrectProperties * ccProps;
//    ColorCorrection * colorCorrection;
};

#endif // IFSEARCH_H

