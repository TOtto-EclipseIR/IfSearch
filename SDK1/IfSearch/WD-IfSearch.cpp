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

#include "../FSBridge/FSDirectBridge.h"

IfSearch::IfSearch(int argc, char *argv[])
    : QApplication(argc, argv, false)
    , matchSettings(INDI::EigenFaceSearchSettings::CasualMatch, this)
    , searchSettings(INDI::EigenFaceSearchSettings::FormalSearch, this)
    , version(VER_MAJOR, VER_MINOR, VER_BRANCH,
              VER_RELEASE, VER_STRING, VER_COPYRIGHT,
              VER_ORGNAME, "IfSearch")
{
    camera = 0;
    paused = true;
    pausePending = false;
    ffdBusy = true;
    avgFace = 0;
    ffd = 0;
    eigenFace = 0;
    eigenMatcher = 0;
    eigenData = 0;
    eigenParms = 0;
    hotdir = 0;
    enrollDir = 0;
    searchDir = 0;
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

    appSettings		= DDT::Settings::newSettings("EclipseIR", "SDKconsole. ", this); NULLPTR(appSettings);
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
    infoSetting		= new DDT::InfoOutputSetting("Options/Message");
    appSettings->setInfoOutputSetting(infoSetting);
    optNoPrompt		= new DDT::Setting(appSettings, tr("Options/NoPrompt", "config"), true);
    optShutdown		= new DDT::Setting(appSettings, tr("Options/Shutdown", "config"), false, DDT::Settings::Volatile);
    appSettings->setValue(optShutdown->keyName(), true);
    optInput		= new DDT::Setting(appSettings, tr("Input/URL", "config"), QString(), DDT::Settings::Volatile);
    optPause		= new DDT::Setting(appSettings, tr("Input/Pause", "config"), false, DDT::Settings::Volatile);
    optRestartSecs  = new DDT::Setting(appSettings, tr("Input/RestartSecs", "config"), 10, DDT::Settings::Volatile);
    optLogFile		= new DDT::Setting(appSettings, tr("Output/LogFile", "config"), "./log/@.log");
# ifdef _DEBUG
    optLogDetail	= new DDT::Setting(appSettings, tr("Output/LogDetail", "config"), "Detail");
# else
    optLogDetail	= new DDT::Setting(appSettings, tr("Output/LogDetail", "config"), "Info");
# endif

    optMarkEyes = new DDT::Setting(appSettings, tr("Output/MarkAllEyes", "config"), true, DDT::Settings::Volatile);
    optMarkAllEyeColor = new DDT::Setting(appSettings, tr("Output/MarkAllEyeColor", "config"), QColor(), DDT::Settings::Volatile);
    optMarkAllColor = new DDT::Setting(appSettings, tr("Output/MarkAllColor", "config"), QColor(), DDT::Settings::Volatile);
    optMarkFaceColor= new DDT::Setting(appSettings, tr("Output/MarkFaceColor", "config"), Qt::yellow, DDT::Settings::Volatile);
    optMarkEyeRoiColor= new DDT::Setting(appSettings, tr("Output/MarkEyeRoiColor", "config"), QColor(), DDT::Settings::Volatile);
    optMarkBadFaceColor	= new DDT::Setting(appSettings, tr("Output/MarkBadFaceColor", "config"), Qt::green, DDT::Settings::Volatile);
    optMarkNoEyesColor	= new DDT::Setting(appSettings, tr("Output/MarkNoEyesColor", "config"), Qt::blue, DDT::Settings::Volatile);
    optMarkEyeColor	= new DDT::Setting(appSettings, tr("Output/MarkEyeColor", "config"), Qt::yellow, DDT::Settings::Volatile);
    optMarkBackgroundColor	= new DDT::Setting(appSettings, tr("Output/MarkBackgroundColor", "config"), QColor(), DDT::Settings::Volatile);
    optMarkBackgroundTransparency = new DDT::Setting(appSettings, tr("Output/MarkBackgroundTransparency", "config"), 100, DDT::Settings::Volatile);
    optMarkOverCrop	= new DDT::Setting(appSettings, tr("Output/MarkOverCrop", "config"), 175, DDT::Settings::Volatile);
    optMarkOverMark	= new DDT::Setting(appSettings, tr("Output/MarkOverMark", "config"), 150, DDT::Settings::Volatile);
    optMarkClothes	= new DDT::Setting(appSettings, tr("Output/MarkClothes", "config"), false, DDT::Settings::Volatile);
    optAppendPersonId= new DDT::Setting(appSettings, tr("Match/AppendPersonId", "config"), false, DDT::Settings::Volatile);
    optMarkBackgroundFile = new DDT::Setting(appSettings, tr("Output/MarkBackgroundFile", "config"), QString(), DDT::Settings::Volatile);
    optForceMarked = new DDT::Setting(appSettings, tr("Output/ForceMarked", "config"), false, DDT::Settings::Volatile);
    optWriteFaceInfo = new DDT::Setting(appSettings, tr("Output/WriteFaceInfo", "config"), true, DDT::Settings::Volatile);
    optDetectEnable	= new DDT::Setting(appSettings, tr("Detect/Enable", "config"), false, DDT::Settings::Volatile);
    optMatchEnable	= new DDT::Setting(appSettings, tr("Match/Enable", "config"), false, DDT::Settings::Volatile);
    optGenerateEnable = new DDT::Setting(appSettings, tr("Generate/Enable", "config"), false, DDT::Settings::Volatile);
    optGenerateFactor = new DDT::Setting(appSettings, tr("Generate/Factor", "config"), 3.0);
    optClothesEnable = new DDT::Setting(appSettings, tr("Clothes/Enable", "config"), false, DDT::Settings::Volatile);
    optHeightEnable = new DDT::Setting(appSettings, tr("Height/Enable", "config"), false, DDT::Settings::Volatile);
    // optHeightCalibrateDir = new DDT::Setting(appSettings, tr("Height/CalibrateDir", "config"), false, DDT::Settings::Volatile);
    optResolveEnable = new DDT::Setting(appSettings, tr("Resolve/Enable", "config"), false, DDT::Settings::Volatile);
    optFaceColorEnable = new DDT::Setting(appSettings, tr("FaceColor/Enable", "config"), false, DDT::Settings::Volatile);
    //	optFaceColorRegionScale = new DDT::Setting(appSettings, tr("FaceColor/RegionScale", "config"), 100, DDT::Settings::Volatile);
    optEnrollCommand= new DDT::Setting(appSettings, tr("Enroll/Command", "config"), QString(), DDT::Settings::Volatile);
    optSearchCommand= new DDT::Setting(appSettings, tr("Search/Command", "config"), QString(), DDT::Settings::Volatile);
    optRetrieveCommand= new DDT::Setting(appSettings, tr("Retrieve/Command", "config"), QString(), DDT::Settings::Volatile);
    optInputOverCrop = new DDT::Setting(appSettings, tr("Detect/InputOverCrop", "config"), 0, DDT::Settings::Volatile);
    optResolveMin = new DDT::Setting(appSettings, tr("Resolve/MinConfidence", "config"), 0, DDT::Settings::Volatile);
    optResolveMax = new DDT::Setting(appSettings, tr("Resolve/MaxConfidence", "config"), 0, DDT::Settings::Volatile);
    optSourceChanged = new DDT::Setting(appSettings, tr("Source/Changed"), false, DDT::Settings::Volatile);
#ifdef ENABLE_WATCHDOG
    optWatchDogValue = new DDT::Setting(appSettings, tr("Options/WatchDogValue"), QString(), DDT::Settings::Volatile);
    appSettings->setValue(optWatchDogValue->keyName(), QString("Initializing"));
#endif

    writer = new DDT::FileWriter(appSettings, QString(), this);
    writer->setImageCache(&imageCache);
    fwpCapture		= writer->newProfile("Capture", DDT::FileWriter::CaptureImage | DDT::FileWriter::TempAndRename);
    fwpCapture2		= writer->newProfile("Capture2");
    fwpMarked		= writer->newProfile("Marked", DDT::FileWriter::TempAndRename);
    fwpNoMark		= writer->newProfile("NoMark");
    fwpUnMarked		= writer->newProfile("UnMarked");
    fwpMarkedFace	= writer->newProfile("MarkedFace", DDT::FileWriter::FaceImage);
    fwpRecon        = writer->newProfile("Recon", DDT::FileWriter::FaceImage);
    fwpVector       = writer->newProfile("Vector", DDT::FileWriter::FaceImage);
    fwpNoFace		= writer->newProfile("NoFace");
    fwpNoEyes		= writer->newProfile("NoEyes");
    fwpBadFace		= writer->newProfile("BadFace");
    fwpNoMatch		= writer->newProfile("NoMatch", DDT::FileWriter::FaceImage);
    fwpMatch		= writer->newProfile("Match", DDT::FileWriter::Copy);
    fwpFace         = writer->newProfile("Face", DDT::FileWriter::FaceImage);
    fwpFaceCache	= writer->newProfile("FaceCache", DDT::FileWriter::FaceImage | DDT::FileWriter::Cache);
    fwpBody         = writer->newProfile("Body");
    fwpClothes		= writer->newProfile("Clothes");
    fwpNoClothes	= writer->newProfile("NoClothes");
    fwpClothesFace	= writer->newProfile("ClothesFace");
    fwpHeight		= writer->newProfile("Height");
    fwpNoHeight		= writer->newProfile("NoHeight");
    fwpHeightFace	= writer->newProfile("HeightFace");
    fwpXml          = writer->newProfile("Xml", DDT::FileWriter::XmlText);
    fwpImage		= writer->newProfile("Image");
    fwpRetrieve		= writer->newProfile("Retrieve", DDT::FileWriter::Copy, "Retrieve/OutputDir");
    fwpRetrRecon	= writer->newProfile("Retrieve", DDT::FileWriter::FaceImage, "Retrieve/ReconDir");
    fwpSearch		= writer->newProfile("Search", DDT::FileWriter::Copy, "Search/OutputDir");
    fwpDetect		= writer->newProfile("Detect", 0, "Detect/OutputDir");
    fwpGenerate		= writer->newProfile("Generate", DDT::FileWriter::FaceImage, "Generate/OutputDir");
    fwpSkin         = writer->newProfile("Skin", 0, "Detect/SkinDir");
    fwpCharcol		= writer->newProfile("Charcol", 0, "Detect/CharcolDir");
    fwpResolveMarked= writer->newProfile("ResolveMarked", DDT::FileWriter::TempAndRename, "Resolve/MarkedDir");
    fwpResolveFace	= writer->newProfile("ResolveFace", DDT::FileWriter::FaceImage, "Resolve/FaceDir");
    fwpNoFaceColor  = writer->newProfile("NoFaceColor", DDT::FileWriter::FaceImage, "FaceColor/NoOutputDir");
#ifdef ENABLE_AVGFACE
    fwpAvgFace      = writer->newProfile("AvgFace", DDT::FileWriter::FaceImage, "AvgFace/OutputDir");
    optAvgFaceEnable = new DDT::Setting(appSettings, tr("AvgFace/Enable", "config"), false);
    optAvgFaceMinConsistency = new DDT::Setting(appSettings, tr("AvgFace/MinConsistency", "config"), 700);
#endif

    skinDetector = new SkinDetector(SkinDetector::Simple);
    skinMatcher = new SkinMatcher;
    clothesMatchProperties = new ClothesMatchProperties(this);
    clothesMatcher = new ClothesMatcher(clothesMatchProperties);


    QTimer::singleShot(0, this, SLOT(init()));
} // c'tor

IfSearch::~IfSearch()
{
    DDT::Info::flush();
}


DDT::Return IfSearch::writeXmlResult(QPair<QString,INDI::DetectorResult> face,
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
    foreach (INDI::EigenFaceResult person, resList)
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

        foreach(INDI::EigenFaceResultFace res, person.results())
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
    return DDT::Return();
} // writeXmlResult(person)

DDT::Return IfSearch::writeMatches(const EigenFaceSearchResultList & resList)
{
    DDT::Return rtn;
    int rank = 0;

    foreach (EigenFaceSearchPerson res, resList)
    {
        QString enrolledImageFileName = faceBase->enrolledImageName(res.bestFaceKey());
        idGenerator.setRank(++rank);
        idGenerator.setConfidence(res.getConfidence());
        idGenerator.setTier(res.getTier().indicator());
        QString baseName = idGenerator.face("Match");
        RETURN(fwpMatch->write(QFile(enrolledImageFileName), baseName));
    } // foreach

    return rtn;
} // writeMatches(face)

DDT::Return IfSearch::writeOutputImage(QPair<QString,INDI::DetectorResult> face,
                                       int consistency,
                                       QImage normImage,
                                       const EigenFaceSearchResultList & resList)
{
    static int pos[] = { 0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4 };

    QImage outputImage(normImage.size() * 2, normImage.format());
    outputImage.fill(Qt::black);
    DDT::ImageMarker outputMarker(&outputImage);
    QSize thumbSize = normImage.size() / 2;

    QColor consistencyColor = optMarkBadFaceColor->value<QColor>();
    if ( ! consistencyColor.isValid()) consistencyColor = Qt::green;
    DDT::ImageMarker normMarker(&normImage);
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
            DDT::ImageMarker thumbMarker(&thumbImage);
            QString id = faceBase->personId(res.getPersonKey());
            if (id.isEmpty())
                id = faceBase->faceId(res.bestFaceKey());
            thumbMarker.title(tr("M%1%3 %2")
                              .arg(res.getConfidence())
                              .arg(id)
                              .arg(res.getTier().indicator()));
            thumbMarker.score(res.getConfidence(), 4, res.getTier().color(),
                              Qt::white, Qt::black);
            thumbMarker.end();
            outputMarker.drawImage(QPoint(pos[i] % 4 * thumbSize.width(),
                                          pos[i] / 4 * thumbSize.height()), thumbImage);
        }
        ++i;
    }
    outputMarker.end();
    RETURN(fwpImage->write(outputImage, idGenerator.face("Image")));
    return DDT::Return();
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

    quint64 installedDays = (msBase - msInstall) / (24 * 3600000ULL);
    qint64 remainingDays = daysToLive - installedDays;

    if (remainingDays < 0)
    {
        ERROR("Your EclipseIR SDK license has expired");
        INFO("Please mailto:sales@EclipseIR.com for an update");
        QTimer::singleShot(0, qApp, SLOT(done()));
        return false;
    }
    else if (remainingDays < 30)
    {
        WARNING("Your EclipseIR SDK license will expire in less than a month");
        INFO("Please mailto:sales@EclipseIR.com for an update");
    }
/*
    else if (msLast - msBase > 2 * 24 * 3600000ULL)
    {
        ERROR("Is the world spinning backwards?");
        INFO("Please mailto:sales@EclipseIR.com for an update");
        QTimer::singleShot(0, qApp, SLOT(done()));
    }
*/
    return true;
}
