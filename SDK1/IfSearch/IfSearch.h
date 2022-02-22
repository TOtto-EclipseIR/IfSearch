#ifndef IFSEARCH_H
#define IFSEARCH_H

#ifndef QT_NO_DEBUG
//#define ENABLE_AVGFACE
#endif

#include <QCoreApplication>
#include <QStringList>

#include <DetectorResult.h>
#include <EigenFaceTemplate.h>
#include <EigenFaceSimilarity.h>
#include <EigenFaceStatus.h>
#include <EigenFaceSearchSettings.h>
#include <EyeDetector.h>
#include <FrontalFaceDetector.h>
#include <HaarDetector.h>
#include <ImageCache.h>
class QFileSystemWatcher;

#include <Eyes.h>
#include <TransformProperties.h>

#include "../../INDI2/oldEIRlibs/eirTypes/MillisecondTime.h"
#include "../../INDI2/oldEIRlibs/eirCore/IdGenerator.h"
#include "../../INDI2/oldEIRlibs/eirFile/CsvWriter.h"
#include "../../INDI2/oldEIRlibs/eirIb2/FramePerformance.h"
#include "../../INDI2/oldEIRlibs/eirIb2/FrameStatistics.h"

class FileWriteProfile;
class FileWriter;
class ImageSource;
class InfoOutputSetting;
class InputHotdir;
class Settings;
class Setting;

class EigenFaceData;
class EigenFaceGenerator;
class EigenFaceParameters;
class EigenFaceSimilarity;
class EyeDetector;
class FrontalFaceDetector;
class FaceBase;

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

#include "Detector.h"

class IfSearch : public QCoreApplication
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
private:
    Setting * optWatchDogValue;
    WatchDog * _watchdog;
#endif

private:
    bool initLicense(void) const;
    Return initEigenFace(void);
    Return writeOutputImage(QPair<QString,DetectorResult> face,
                                 int consistency,
                                 QImage normImage,
                                 const EigenFaceSearchResultList & resList);
    Return writeXmlResult(QPair<QString,DetectorResult> face,
                               const EigenFaceSearchResultList & resList);
    Return writeMatches(const EigenFaceSearchResultList & resList);

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
    void reload(void);
    void hotdirReady(void);
    void hotdirEmpty(void);
    void grabbed(QString ImageId);
    void changed(QString key);
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
    void doSimilarity(void);
    void doVerify(void);
    void doAuthenticate(void);
    void doRetrieve(void);
    void doRetrieveAll(void);
    void doStatus(void);
    void doPersonReport(void);
    void searchScanDir(void);
    void searchGrab(void);
    void searchOrVerify(void);
    void similarityScanDir(void);
    void similarityGrab(void);
    void similarityProcess(void);
    void similarityResults(void);
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
    void logRollover(void);

private:
    VersionInfo version;
    Settings * appSettings;
    InfoOutputSetting * infoSetting;
    Setting * optNoPrompt;
    Setting * optInput;
    Setting * optPause;
    Setting * optRestartSecs;
    Setting * optLogFile;
    Setting * optLogDetail;
    Setting * optDetectEnable;
    Setting * optMatchEnable;
    Setting * optGenerateEnable;
    Setting * optGenerateFactor;
    Setting * optFaceColorEnable;
    Setting * optHeightEnable;
    Setting * optClothesEnable;
    Setting * optResolveEnable;
    Setting * optMarkAllColor;
    Setting * optMarkEyes;
    Setting * optMarkAllEyeColor;
    Setting * optMarkFaceColor;
    Setting * optMarkEyeRoiColor;
    Setting * optMarkBadFaceColor;
    Setting * optMarkNoEyesColor;
    Setting * optMarkEyeColor;
    Setting * optMarkBackgroundColor;
    Setting * optMarkBackgroundTransparency;
    Setting * optMarkOverCrop;
    Setting * optMarkOverMark;
    Setting * optMarkBackgroundFile;
    Setting * optMarkClothes;
    Setting * optAppendPersonId;
    Setting * optWriteFaceInfo;
    Setting * optEnrollCommand;
    Setting * optSearchCommand;
    Setting * optRetrieveCommand;
    Setting * optInputOverCrop;
    Setting * optForceMarked;
    Setting * optForceHeight;
    Setting * optShutdown;
    Setting * optResolveMin;
    Setting * optResolveMax;
    Setting * optSourceChanged;
#ifdef ENABLE_AVGFACE
    Setting * optAvgFaceEnable;
    Setting * optAvgFaceMinConsistency;
    FileWriteProfile * fwpAvgFace;
#endif

    QString eigenFaceDataDir;
    //    QString eigenFaceParmDir;
    QString faceBaseBaseDir;
    QString detectorsXml;
    int faceBaseMaxLoad;
    MillisecondDelta faceBaseReload_msd;
    QTimer * reloadTimer;

    QTimer * rolloverTimer;

    ImageSource * camera;

    MillisecondTime streamBase_mst;
    bool paused;
    bool pausePending;
    bool ffdBusy;
    AverageFace * avgFace;
    FrontalFaceDetector * ffd;
    EigenFaceGenerator * eigenFace;
    EigenFaceData * eigenData;
    EigenFaceParameters * eigenParms;
    EigenFaceSearcher * eigenMatcher;
    EigenFaceSearchSettings matchSettings;
    EigenFaceSearchSettings searchSettings;
    FaceBase * faceBase;
    InputHotdir * hotdir;
    ImageCache imageCache;
    //QString searchId;
    QImage markedImage;
    QImage grabImage;
    bool isMarked;
    bool isHeight;
    int FramesProcessed;
    int NullFrames;
    int FacesProcessed;
    int numSearches;
    int msecSearches;
    QString commandMode;

    IdGenerator idGenerator;
    FileWriter * writer;
    FileWriteProfile * fwpCapture;
    FileWriteProfile * fwpCapture2;
    FileWriteProfile * fwpMarked;
    FileWriteProfile * fwpNoMark;
    FileWriteProfile * fwpUnMarked;
    FileWriteProfile * fwpDetect;
    FileWriteProfile * fwpGenerate;
    FileWriteProfile * fwpSkin;
    FileWriteProfile * fwpCharcol;
    FileWriteProfile * fwpMarkedFace;
    FileWriteProfile * fwpRecon;
    FileWriteProfile * fwpVector;
    FileWriteProfile * fwpNoFace;
    FileWriteProfile * fwpNoEyes;
    FileWriteProfile * fwpBadFace;
    FileWriteProfile * fwpNoMatch;
    FileWriteProfile * fwpMatch;
    FileWriteProfile * fwpFace;
    FileWriteProfile * fwpBody;
    FileWriteProfile * fwpClothes;
    FileWriteProfile * fwpNoClothes;
    FileWriteProfile * fwpClothesFace;
    FileWriteProfile * fwpHeight;
    FileWriteProfile * fwpNoHeight;
    FileWriteProfile * fwpHeightFace;
    FileWriteProfile * fwpFaceCache;
    FileWriteProfile * fwpXml;
    FileWriteProfile * fwpImage;
    FileWriteProfile * fwpRetrieve;
    FileWriteProfile * fwpRetrRecon;
    FileWriteProfile * fwpSearch;
    FileWriteProfile * fwpSimilarity;
    FileWriteProfile * fwpResolveFace;
    FileWriteProfile * fwpResolveMarked;
    FileWriteProfile * fwpNoFaceColor;
    QMap<QString,FileWriteProfile *> fwpsFaceColor;

    QList<QPair<QString,DetectorResult> > pendingFaces;
    QString resultsDelimiter;

    int enrollTimeout;
    bool enrollPersonMode;
    EigenFaceStatus enroll_faceStatus;
    QDir enrollDir;
    int enrollPersonKey;
    QString enrollPersonId;
    QStringList enrollPending;
    QString enrollResults;

    int searchTimeout;
    QDir searchDir;
    int searchPersonKey;
    QString searchPersonId;
    EigenFaceTemplate searchTemplate;
    EigenFaceSimilarity * searchSimilarity;
    EigenFaceSimilarityResultList similarityResultsList;
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

