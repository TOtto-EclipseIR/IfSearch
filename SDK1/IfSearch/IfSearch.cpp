#include "IfSearch.h"
#include "version.h"
#include "../../common/version.h"

#include <QStringList>
#include <QTimer>

#include <EigenFaceGenerator.h>
#include <EigenFaceSearchResultList.h>
#include <EigenFaceSearchTier.h>
#include <EigenFaceSearchPerson.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <FileWriter.h>
#include <ImageMarker.h>
#include <InfoMacros.h>
#include <Return.h>
#include <Settings.h>
//#include <Trace.h>

#include <ClothesMatchProperties.h>
#include <ClothesMatcher.h>
#include <SkinDetector.h>
#include <SkinMatchProperties.h>
#include <SkinMatcher.h>

IfSearch::IfSearch(int argc, char *argv[])
    : QCoreApplication(argc, argv, false)
    , matchSettings(EigenFaceSearchSettings::CasualMatch, this)
    , searchSettings(EigenFaceSearchSettings::FormalSearch, this)
    , version(VER_MAJOR, VER_MINOR, VER_BRANCH,
              VER_RELEASE, VER_STRING, VER_COPYRIGHT,
              VER_ORGNAME, VER_APPNAME)
    , reloadTimer(0)
    , rolloverTimer(0)
{
    camera = 0;
    paused = true;
    pausePending = false;
    ffdBusy = true;
    avgFace = 0;
    ffd = 0;
    eigenFace = 0;
    eigenMatcher = 0;
    searchSimilarity = 0;
    eigenData = 0;
    eigenParms = 0;
    hotdir = 0;
//    enrollDir = 0;
  //  searchDir = 0;
    authWatcher = 0;
    resolver = 0;
    heightGrid = 0;
    numSearches = 0;
    msecSearches = 0;
    frameStatistics = 0;
    framePerformance = 0;
    fpWriter = 0;
#ifdef ENABLE_WATCHDOG
    _watchdog = 0;
#endif

    appSettings		= Settings::newSettings(this); NULLPTR(appSettings);
    appSettings->setValue("Enroll/Command", QString());
    appSettings->setValue("Retrieve/Command", QString());
    appSettings->setValue("Search/Command", QString());
    appSettings->setValue("Output/FramesProcessed",
                          QString::number(FramesProcessed=0));
    appSettings->setValue("Output/NullFrames",
                          QString::number(NullFrames=0));
    appSettings->setValue("Output/FacesProcessed",
                          QString::number(FacesProcessed=0));
    appSettings->setValue("Input/Processing", QString());
    infoSetting		= new InfoOutputSetting("Options/Message");
    appSettings->setInfoOutputSetting(infoSetting);
    optNoPrompt		= new Setting(appSettings, tr("Options/NoPrompt", "config"), true);
    optShutdown		= new Setting(appSettings, tr("Options/Shutdown", "config"), false, Settings::Volatile);
    appSettings->setValue(optShutdown->keyName(), true);
    optInput		= new Setting(appSettings, tr("Input/URL", "config"), QString(), Settings::Volatile);
    optPause		= new Setting(appSettings, tr("Input/Pause", "config"), false, Settings::Volatile);
    optRestartSecs  = new Setting(appSettings, tr("Input/RestartSecs", "config"), 10, Settings::Volatile);
    optLogFile		= new Setting(appSettings, tr("Output/LogFile", "config"), "./log/@.log");
# ifdef _DEBUG
    optLogDetail	= new Setting(appSettings, tr("Output/LogDetail", "config"), "Detail");
# else
    optLogDetail	= new Setting(appSettings, tr("Output/LogDetail", "config"), "Info");
# endif

    optMarkEyes = new Setting(appSettings, tr("Output/MarkAllEyes", "config"), true, Settings::Volatile);
    optMarkAllEyeColor = new Setting(appSettings, tr("Output/MarkAllEyeColor", "config"), QColor(), Settings::Volatile);
    optMarkAllColor = new Setting(appSettings, tr("Output/MarkAllColor", "config"), QColor(), Settings::Volatile);
    optMarkFaceColor= new Setting(appSettings, tr("Output/MarkFaceColor", "config"), int(Qt::yellow), Settings::Volatile);
    optMarkEyeRoiColor= new Setting(appSettings, tr("Output/MarkEyeRoiColor", "config"), QColor(), Settings::Volatile);
    optMarkBadFaceColor	= new Setting(appSettings, tr("Output/MarkBadFaceColor", "config"), int(Qt::green), Settings::Volatile);
    optMarkNoEyesColor	= new Setting(appSettings, tr("Output/MarkNoEyesColor", "config"), int(Qt::blue), Settings::Volatile);
    optMarkEyeColor	= new Setting(appSettings, tr("Output/MarkEyeColor", "config"), int(Qt::yellow), Settings::Volatile);
    optMarkBackgroundColor	= new Setting(appSettings, tr("Output/MarkBackgroundColor", "config"), QColor(), Settings::Volatile);
    optMarkBackgroundTransparency = new Setting(appSettings, tr("Output/MarkBackgroundTransparency", "config"), 100, Settings::Volatile);
    optMarkOverCrop	= new Setting(appSettings, tr("Output/MarkOverCrop", "config"), 175, Settings::Volatile);
    optMarkOverMark	= new Setting(appSettings, tr("Output/MarkOverMark", "config"), 150, Settings::Volatile);
    optMarkClothes	= new Setting(appSettings, tr("Output/MarkClothes", "config"), false, Settings::Volatile);
    optAppendPersonId= new Setting(appSettings, tr("Match/AppendPersonId", "config"), false, Settings::Volatile);
    optMarkBackgroundFile = new Setting(appSettings, tr("Output/MarkBackgroundFile", "config"), QString(), Settings::Volatile);
    optForceMarked = new Setting(appSettings, tr("Output/ForceMarked", "config"), false, Settings::Volatile);
    optForceHeight = new Setting(appSettings, tr("Output/ForceHeight", "config"), false, Settings::Volatile);
    optWriteFaceInfo = new Setting(appSettings, tr("Output/WriteFaceInfo", "config"), true, Settings::Volatile);
    optDetectEnable	= new Setting(appSettings, tr("Detect/Enable", "config"), false, Settings::Volatile);
    optMatchEnable	= new Setting(appSettings, tr("Match/Enable", "config"), false, Settings::Volatile);
    optGenerateEnable = new Setting(appSettings, tr("Generate/Enable", "config"), false, Settings::Volatile);
    optGenerateFactor = new Setting(appSettings, tr("Generate/Factor", "config"), 3.0);
    optClothesEnable = new Setting(appSettings, tr("Clothes/Enable", "config"), false, Settings::Volatile);
    optHeightEnable = new Setting(appSettings, tr("Height/Enable", "config"), false, Settings::Volatile);
    // optHeightCalibrateDir = new Setting(appSettings, tr("Height/CalibrateDir", "config"), false, Settings::Volatile);
    optResolveEnable = new Setting(appSettings, tr("Resolve/Enable", "config"), false, Settings::Volatile);
    optFaceColorEnable = new Setting(appSettings, tr("FaceColor/Enable", "config"), false, Settings::Volatile);
    //	optFaceColorRegionScale = new Setting(appSettings, tr("FaceColor/RegionScale", "config"), 100, Settings::Volatile);
    optEnrollCommand= new Setting(appSettings, tr("Enroll/Command", "config"), QString(), Settings::Volatile);
    optSearchCommand= new Setting(appSettings, tr("Search/Command", "config"), QString(), Settings::Volatile);
    optRetrieveCommand= new Setting(appSettings, tr("Retrieve/Command", "config"), QString(), Settings::Volatile);
    optInputOverCrop = new Setting(appSettings, tr("Detect/InputOverCrop", "config"), 0, Settings::Volatile);
    optResolveMin = new Setting(appSettings, tr("Resolve/MinConfidence", "config"), 0, Settings::Volatile);
    optResolveMax = new Setting(appSettings, tr("Resolve/MaxConfidence", "config"), 0, Settings::Volatile);
    optSourceChanged = new Setting(appSettings, tr("Source/Changed"), false, Settings::Volatile);
#ifdef ENABLE_WATCHDOG
    optWatchDogValue = new Setting(appSettings, tr("Options/WatchDogValue"), QString(), Settings::Volatile);
    appSettings->setValue(optWatchDogValue->keyName(), QString("Initializing"));
#endif

    writer = new FileWriter(appSettings, QString(), this);
    writer->setImageCache(&imageCache);
    writer->setCacheDirs(appSettings->value("Output/CacheDirs","FaceCache").toString());
    fwpCapture      = writer->newProfile("Capture", FileWriter::Flags(FileWriter::CaptureImage | FileWriter::TempAndRename));
    fwpCapture2     = writer->newProfile("Capture2");
    fwpMarked       = writer->newProfile("Marked", FileWriter::TempAndRename);
    fwpNoMark       = writer->newProfile("NoMark");
    fwpUnMarked     = writer->newProfile("UnMarked");
    fwpMarkedFace   = writer->newProfile("MarkedFace");
    fwpRecon        = writer->newProfile("Recon");
    fwpVector       = writer->newProfile("Vector");
    fwpNoFace       = writer->newProfile("NoFace");
    fwpNoEyes       = writer->newProfile("NoEyes");
    fwpBadFace      = writer->newProfile("BadFace");
    fwpNoMatch      = writer->newProfile("NoMatch", FileWriter::FaceImage);
    fwpMatch        = writer->newProfile("Match", FileWriter::Copy);
    fwpFace         = writer->newProfile("Face", FileWriter::FaceImage);
    fwpFaceCache    = writer->newProfile("FaceCache", FileWriter::Flags(FileWriter::FaceImage | FileWriter::Cache));
    fwpBody         = writer->newProfile("Body");
    fwpClothes      = writer->newProfile("Clothes");
    fwpNoClothes    = writer->newProfile("NoClothes");
    fwpClothesFace  = writer->newProfile("ClothesFace");
    fwpHeight       = writer->newProfile("Height");
    fwpNoHeight     = writer->newProfile("NoHeight");
    fwpHeightFace   = writer->newProfile("HeightFace");
    fwpXml          = writer->newProfile("Xml", FileWriter::XmlText);
    fwpImage        = writer->newProfile("Image");
    fwpRetrieve     = writer->newProfile("Retrieve", FileWriter::Copy, "Retrieve/OutputDir");
    fwpRetrRecon    = writer->newProfile("Retrieve", FileWriter::$nullFlag, "Retrieve/ReconDir");
    fwpSearch       = writer->newProfile("Search", FileWriter::Copy, "Search/OutputDir");
    fwpSimilarity   = writer->newProfile("Similarity", FileWriter::$nullFlag, "Search/SimilarityDir");
    fwpDetect       = writer->newProfile("Detect", FileWriter::$nullFlag, "Detect/OutputDir");
    fwpGenerate     = writer->newProfile("Generate", FileWriter::$nullFlag, "Generate/OutputDir");
    fwpSkin         = writer->newProfile("Skin", FileWriter::$nullFlag, "Detect/SkinDir");
    fwpCharcol      = writer->newProfile("Charcol", FileWriter::$nullFlag, "Detect/CharcolDir");
    fwpResolveMarked= writer->newProfile("ResolveMarked", FileWriter::TempAndRename, "Resolve/MarkedDir");
    fwpResolveFace  = writer->newProfile("ResolveFace", FileWriter::$nullFlag, "Resolve/FaceDir");
    fwpNoFaceColor  = writer->newProfile("NoFaceColor", FileWriter::$nullFlag, "FaceColor/NoOutputDir");
#ifdef ENABLE_AVGFACE
    fwpAvgFace      = writer->newProfile("AvgFace", FileWriter::FaceImage, "AvgFace/OutputDir");
    optAvgFaceEnable = new Setting(appSettings, tr("AvgFace/Enable", "config"), false);
    optAvgFaceMinConsistency = new Setting(appSettings, tr("AvgFace/MinConsistency", "config"), 700);
#endif

    skinDetector = new SkinDetector(SkinDetector::Simple);
    skinMatcher = new SkinMatcher;
#ifdef ENABLE_CLOTHES
    clothesMatchProperties = new ClothesMatchProperties(this);
    clothesMatcher = new ClothesMatcher(clothesMatchProperties);
#endif

    QTimer::singleShot(0, this, SLOT(init()));
} // c'tor

IfSearch::~IfSearch()
{
    Info::flush();
}


Return IfSearch::writeXmlResult(QPair<QString,DetectorResult> face,
                                const EigenFaceSearchResultList & resList)
{
    /*
    QDomDocument doc("INDIfacePersonResult");
    QDomElement deMain = doc.createElement("PersonResultList");
    doc.appendChild(deMain);

    QDomElement deFace = doc.createElement("SearchFace");
    deMain.appendChild(deFace);
    deFace.setAttribute("ImageId", face.first);
    deFace.setAttribute("FaceCenterX", face.second.rectangle().center().x());
    deFace.setAttribute("FaceCenterY", face.second.rectangle().center().y());
    deFace.setAttribute("FaceWidth", face.second.rectangle().width());
    deFace.setAttribute("FaceHeight", face.second.rectangle().height());
    if (fwpCapture->isActive())
        deFace.setAttribute("CaptureFile", fwpCapture->filePath(face.first));
    if (fwpFace->isActive())
        deFace.setAttribute("FaceFile", fwpFace->filePath(idGenerator.face("Face")));

    int rank = 0;
    foreach (EigenFaceResult person, resList)
    {
        QDomElement dePerson = doc.createElement(person.personKey() ? "PersonResult" : "UnidentifiedResult");
        deMain.appendChild(dePerson);
        dePerson.setAttribute("Rank", ++rank);
        dePerson.setAttribute("Confidence", person.confidence());
        if (person.personKey())
        {
            dePerson.setAttribute("PersonKey", person.personKey());
            QString personId = faceBase->personId(person.personKey());
            if ( ! personId.isEmpty())
                dePerson.setAttribute("PersonId",  personId);
        }

        foreach(EigenFaceResultFace res, person.results())
        {
            QDomElement de = doc.createElement("ResultEntry");
            dePerson.appendChild(de);
            de.setAttribute("Distance", res.distance());
            de.setAttribute("FaceKey", res.faceKey());
            de.setAttribute("FaceId", faceBase->faceId(res.faceKey(), res.personKey()));
            de.setAttribute("NormalizedImageFile", faceBase->enrolledImageName(res.faceKey()));
        }
    } // foreach

    RETURN(fwpXml->write(doc, idGenerator.face("Xml")));
    */
    return Return();
} // writeXmlResult(person)

Return IfSearch::writeMatches(const EigenFaceSearchResultList & resList)
{
    Return rtn;
    int rank = 0;

    foreach (EigenFaceSearchPerson res, resList)
    {
        QString enrolledImageFileName = faceBase->enrolledImageName(res.bestFaceKey());
        idGenerator.Rank = (++rank);
        idGenerator.Confidence = (res.Confidence);
        idGenerator.Tier = (res.Tier.indicator());
        QString baseName = idGenerator.face("Match");
        RETURN(fwpMatch->write(QFile(enrolledImageFileName), baseName));
    } // foreach

    return rtn;
} // writeMatches(face)

Return IfSearch::writeOutputImage(QPair<QString,DetectorResult> face,
                                  int consistency,
                                  QImage normImage,
                                  const EigenFaceSearchResultList & resList)
{
    static int pos[] = { 0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4 };

    QImage outputImage(normImage.size() * 2, normImage.format());
    outputImage.fill(Qt::black);
    ImageMarker outputMarker(&outputImage);
    QSize thumbSize = normImage.size() / 2;

    QColor consistencyColor = optMarkBadFaceColor->value<QColor>();
    if ( ! consistencyColor.isValid()) consistencyColor = Qt::green;
    ImageMarker normMarker(&normImage);
    normMarker.title(tr("C%1 %2").arg(consistency).arg(face.first));
    normMarker.score(consistency, 6, Qt::black, Qt::white, consistencyColor);
    normMarker.eyes(eigenFace->normalEyes());
    normMarker.end();
    outputMarker.drawImage(QPoint(thumbSize.width(), thumbSize.height()), normImage);

    int i = 0;
    foreach (EigenFaceSearchPerson res, resList)
    {
        if (i > 11)
            break;
        QImage enrolledNormImage = faceBase->enrolledImage(res.bestFaceKey())
                .convertToFormat(QImage::Format_RGB32);
        if ( ! enrolledNormImage.isNull())
        {
            QImage thumbImage = enrolledNormImage.scaled(thumbSize, Qt::KeepAspectRatio);
            ImageMarker thumbMarker(&thumbImage);
            QString id = faceBase->personId(res.PersonKey);
            if (id.isEmpty())
                id = faceBase->faceId(res.bestFaceKey());
            thumbMarker.title(tr("M%1%3 %2")
                              .arg(res.Confidence)
                              .arg(id)
                              .arg(res.Tier.indicator()));
            thumbMarker.score(res.Confidence, 4, res.Tier.color(),
                              Qt::white, Qt::black);
            thumbMarker.end();
            outputMarker.drawImage(QPoint(pos[i] % 4 * thumbSize.width(),
                                          pos[i] / 4 * thumbSize.height()), thumbImage);
        }
        ++i;
    }
    outputMarker.end();
    RETURN(fwpImage->write(outputImage, idGenerator.face("Image")));
    return Return();
} // writeOutputImage()

bool IfSearch::check(const quint64 daysToLive) const
{
    quint64 msBase = QDateTime::currentMSecsSinceEpoch();
    quint64 msInstall = appSettings->value("Options/Install", 0).toULongLong();
    quint64 installedVersion = appSettings->value("Options/Version", 0).toUInt();

    if (0 == installedVersion || installedVersion < version.toDWord())
    {
        msInstall = msBase;
        appSettings->setValue("Options/Install", msInstall);
        appSettings->setValue("Options/Version", version.toDWord());
    }

    if (daysToLive)
    {
        quint64 installedDays = (msBase - msInstall) / (24 * 3600000ULL);
        qint64 remainingDays = daysToLive - installedDays;
        if (remainingDays < 0)
        {
            ERRMSG("Your EclipseIR SDK license has expired");
            INFO("Please mailto:sales@EclipseIR.com for an update");
            QTimer::singleShot(0, qApp, SLOT(done()));
            return false;
        }
        else if (remainingDays < 30)
        {
            WARNING("Your EclipseIR SDK license will expire in less than a month");
            INFO("Please mailto:sales@EclipseIR.com for an update");
        }
    }
    return true;
}
