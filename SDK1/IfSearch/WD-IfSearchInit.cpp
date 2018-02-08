#include "IfSearch.h"

#include <QLibraryInfo>
#include <QTimer>

#include <DDTcore.h>
#include <Detector.h>
#include <EigenFace.h>
#include <EigenFaceData.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceSearcher.h>
#include <EigenFaceSearchTier.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <FileWriter.h>
#include <ImageCache.h>
#include <ImageSource.h>
#include <InfoMacros.h>
#include <INDIef.h>
#include <INDIffd.h>
#include <InputHotdir.h>
#include <Return.h>
#include <Settings.h>

#include <VersionInfo.h>
//#include <color/ColorCorrection.h>
//#include <color/ColorCorrectProperties.h>

#include <Resolver.h>
#include <ClothesMatchProperties.h>
#include <HeightGrid.h>
#include <SkinMatcher.h>
#include <SkinMatchProperties.h>

#include "../FSBridge/FSDirectBridge.h"
#ifdef ENABLE_WATCHDOG
#include <eirExe/WatchDog.h>
#endif

typedef struct _PROCESS_INFORMATION {
  void * hProcess;
  void * hThread;
  quint32  dwProcessId;
  quint32  dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

void IfSearch::init(void)
{
    DDT::Return rtn;
    QStringList qsl;

    appSettings->setPollCountKey("Options/PollCount");
    if (appSettings->value("Output/LogStdout", true).toBool())
        DDT::Info::add(new DDT::InfoOutputFile(stdout));

    QString sDetail = optLogDetail->toString();
    DDT::InfoSeverity logDetail = DDT::InfoSeverity::Info;
    if (0 == sDetail.compare("Null", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Null;
    else if (0 == sDetail.compare("Leave", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Leave;
    else if (0 == sDetail.compare("Enter", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Enter;
    else if (0 == sDetail.compare("Detail", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Detail;
    else if (0 == sDetail.compare("Debug", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Debug;
    else if (0 == sDetail.compare("Info", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Info;
    else if (0 == sDetail.compare("Progress", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Progress;
    else if (0 == sDetail.compare("Warning", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Warning;
    else if (0 == sDetail.compare("Error", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Error;
    else if (0 == sDetail.compare("Fatal", Qt::CaseInsensitive))
        logDetail = DDT::InfoSeverity::Fatal;
    QString logFileName = optLogFile->toString();
    if (0 != logFileName.compare("none", Qt::CaseInsensitive))
    {
        QDir logDir(writer->baseDir());
        if (logFileName.isEmpty())
            logFileName = "./log/IfSearch-@.log";
        QFileInfo fi(logFileName);
        if (fi.isDir())
            logFileName += "/IfSearch-@.log";
        logFileName = logDir.absoluteFilePath(logFileName);
        DDT::Info::add(new DDT::InfoOutputFile(logFileName),
                       logDetail.isValid() ? logDetail : DDT::InfoSeverity::Info);
    }

    DDT::Info::add(infoSetting, DDT::InfoSeverity::Warning,
                                DDT::InfoSeverity::Fatal);
    DDT::Info::start(QThread::NormalPriority);
    PROGRESS("IfSearch %1 built %2", version.toString(),
             version.dateTimeString());
    INFO(version.getCopyright());
    DETAIL("Running ProcessId=%1", applicationPid());
    version.check(0xE3ECE9F0);
    PROGRESS("Running Qt Version %1 built %2", qVersion(), QLibraryInfo::buildDate());
    detectorsXml     = appSettings->value("Detect/DetectorsXml", "../detectors/Detectors.xml").toString();
    eigenFaceDataDir = appSettings->value("Generate/DataDir", "../data/Face1").toString();
    faceBaseBaseDir  = appSettings->value("FaceBase/BaseDir", "../FaceBase").toString();
    faceBaseMaxLoad  = appSettings->value("FaceBase/MaxLoad", 50).toInt();
#ifdef ENABLE_RELOADMINS
    faceBaseReload.setToMinutes(appSettings->value("FaceBase/ReloadMins", 0.0).toFloat());
#endif


    EigenFaceSearchTier::setColor(EigenFaceSearchTier::Best,
                                 appSettings->value("Output/MarkBestColor",
                                                    EigenFaceSearchTier::color(EigenFaceSearchTier::Best)).value<QColor>());
    EigenFaceSearchTier::setColor(EigenFaceSearchTier::Strong,
                                 appSettings->value("Output/MarkStrongColor",
                                                    EigenFaceSearchTier::color(EigenFaceSearchTier::Strong)).value<QColor>());
    EigenFaceSearchTier::setColor(EigenFaceSearchTier::Possible,
                                 appSettings->value("Output/MarkPossibleColor",
                                                    EigenFaceSearchTier::color(EigenFaceSearchTier::Possible)).value<QColor>());
    EigenFaceSearchTier::setColor(EigenFaceSearchTier::Weak,
                                 appSettings->value("Output/MarkWeakColor",
                                                    EigenFaceSearchTier::color(EigenFaceSearchTier::Weak)).value<QColor>());

    optSearchCommand->setValue(QString());
    optEnrollCommand->setValue(QString());
    optRetrieveCommand->setValue(QString());

#ifdef ENABLE_WATCHDOG
    _watchdog = new WatchDog(this);
#endif

    QTimer::singleShot(0, this, SLOT(start()));
} // init()

void IfSearch::start(void)
{
    DDT::Return rtn;
    QStringList qsl;

    /*--- Setup Clothes Matcher ---*/
    appSettings->objectProperties(clothesMatchProperties, tr("Clothes", "config"), clothesMatchProperties->dynamicPropertyNames(), DDT::Settings::Volatile);

    /*--- Initialize Height Grid ---*/
    heightGrid = new HeightGrid(this);
    NULLPTR(heightGrid);
    connect(heightGrid, SIGNAL(error(QString)), this, SLOT(error(QString)));
    appSettings->objectProperties(heightGrid, tr("Height", "config"), heightGrid->dynamicPropertyNames(), DDT::Settings::Volatile);

    /*--- Setup Skin Matchers ---*/
    appSettings->beginGroup("FaceColor");
    qsl = appSettings->childGroups();
    appSettings->endGroup();
    foreach (QString name, qsl)
    {
        SkinMatchProperties * smp = new SkinMatchProperties(name, this);
        appSettings->objectProperties(smp, "FaceColor/" + name, DDTcore::propertyNames(smp), DDT::Settings::Volatile);
        DDT::FileWriteProfile * fwpOutput = writer->newProfile(name+"-Output", DDT::FileWriter::FaceImage, "FaceColor/"+name+"/OutputDir");
        DDT::FileWriteProfile * fwpMarked = writer->newProfile(name+"-Marked", 0, "FaceColor/"+name+"/MarkedDir");
        // TODO: InputImageDir properties
        skinMatcher->add(name, smp);
        fwpsFaceColor.insert(name+"-Output", fwpOutput);
        fwpsFaceColor.insert(name+"-Marked", fwpMarked);
    }
    // TODO: Global SkinMatcher properties: RegionScale, Regions

    /*--- Color Correction ---*/
//    NULLPTR(ccProps);
//    NULLPTR(colorCorrection);
//    appSettings->objectProperties(&ccProps, tr("ColorCorrection", "config"), ccProps.dynamicPropertyNames());

    /*--- Setup Transform ---*/
    appSettings->objectProperties(&xformProps, tr("PreProcess", "config"), xformProps.dynamicPropertyNames(), DDT::Settings::Volatile);

    /*--- Setup Resolver ---*/
    resolver = new Resolver(this);  NULLPTR(resolver);
    qsl.clear(); qsl << "Quality" << "Consistency" << "FaceColor"
                     << "UpperClothes" << "LowerClothes" << "Height";
    foreach (IdString name, qsl)
    {
        DDT::Setting * opt = new DDT::Setting(appSettings, "Resolve/" + name, 0, DDT::Settings::Volatile);
        int weight = opt->toInt();
        resolver->add(name, weight);
    }

    /*--- Setup Performance Writers ---*/
    QString fpFileName = appSettings->value("Output/FramePerformanceFile", QString()).toString();
    if ( ! fpFileName.isEmpty())
    {
        fpFileName = writer->baseDir().absoluteFilePath(fpFileName);
        fpWriter = new CsvWriter(fpFileName);
        NULLPTR(fpWriter);
        if (fpWriter->isOpen())
        {
            frameStatistics = new FrameStatistics;
            NULLPTR(frameStatistics);
            framePerformance = new FramePerformance;
            NULLPTR(framePerformance);
            fpWriter->add(framePerformance);
            fpWriter->add(frameStatistics);
            fpWriter->header();
            PROGRESS("FramePerformanceFile %1 started", fpFileName);
        }
        else
        {
            ERROR("Unable to write FramePerformanceFile %1: %2", fpFileName, fpWriter->errorString());
            delete fpWriter;
            fpWriter = 0;
        }

    }


    appSettings->objectProperty(appSettings,
                                tr("Options", "config"),
                                tr("UpdateMsec", "config"),
                                DDT::Settings::Volatile);
    connect(appSettings, SIGNAL(valueChanged(QString)),
            this, SLOT(changed(QString)));
    connect(appSettings, SIGNAL(propertyChanged(QString)),
            this, SLOT(changed(QString)));
    connect(appSettings, SIGNAL(valueChanged(QString)),
            writer, SLOT(settingChanged(QString)));

    connect(writer, SIGNAL(directorySet(QString,QString)),
            this, SLOT(directorySet(QString,QString)));
    connect(writer, SIGNAL(fileWritten(QString)),
            this, SLOT(fileWritten(QString)));
    connect(writer, SIGNAL(error(QString)),
            this, SLOT(error(QString)));

    resultsDelimiter = appSettings->value("Options/Delimiter", "^").toString();
    if ( ! check(365))
        return;

    rtn = initEigenFace();
    RETURN(rtn);
    if (rtn.isError() || rtn.isFalse())
    {
        QTimer::singleShot(0, this, SLOT(done()));
        return;
    }

    PROGRESS("Initializing INDI FaceBase");
    faceBase = new INDI::FaceBase(this, eigenMatcher);	NULLPTR(faceBase);
    INFO("from %1", faceBaseBaseDir);
    rtn = faceBase->initialize(faceBaseBaseDir, faceBaseMaxLoad);
    RETURN(rtn);
    if (rtn.isError() || rtn.isFalse())
    {
        QTimer::singleShot(0, this, SLOT(done()));
        return;
    }

#ifdef ENABLE_RELOADMINS
    if ( ! faceBaseReload.isNull())
        QTimer::singleShot(faceBaseReload, this, SLOT(doReload()));
#endif
#ifdef ENABLE_WATCHDOG
    MillisecondDelta interval;
    interval.setToMinutes(appSettings->value("Options/WatchDogMins", "0.0").toFloat());
    if ( ! interval.isNull())
    {
        _watchdog->setInterval(interval);
        CONNECT(_watchdog, SIGNAL(bite(QVariant,MillisecondTime)),
                this, SLOT(dogBite(QVariant,MillisecondTime)));
    }
#endif

    QTimer::singleShot(0, this, SLOT(run()));
} // start()

void IfSearch::run(void)
{
    FUNCTION();

#ifdef ENABLE_AVGFACE
    if (optAvgFaceEnable->toBool())
        avgFaceInit();
#endif
#ifdef ENABLE_WATCHDOG
    if (_watchdog && _watchdog->isReadyToStart())
    {
        appSettings->setValue(optWatchDogValue->keyName(), "Start");
        _watchdog->start();
    }
#endif
    appSettings->setValue(optShutdown->keyName(), false);
    appSettings->startTimers();
    appSettings->dump(DDT::InfoSeverity::Info);

    writer->start();
    writer->pumpAll();

    if (fwpRecon->isActive())
    {
        QImage avg = eigenData->meanFaceImage();
        fwpRecon->write(avg, "AvgFace");
    }

    changed(optHeightEnable->keyName());
    changed(optInput->keyName());
    changed(optPause->keyName());

    QTimer::singleShot(0, this, SLOT(pulse()));
} // run()


DDT::Return IfSearch::initEigenFace(void)
{
    QStringList qsl;
    DDT::Return rtn;

    QDir xmlDir(qApp->applicationDirPath());
    QFile file(xmlDir.absoluteFilePath(detectorsXml));
    INFO("Reading detectors from " + file.fileName());
    rtn = INDI::HaarDetector::initialize(&file);
    RETURN(rtn);
    if (rtn.isError())
        return rtn;

    PROGRESS("Initializing INDIface Face Detector");
    unsigned faceInterface = appSettings->value("Detect/Interface", 0).toUInt();
    ffd = new INDI::FrontalFaceDetector(faceInterface, this); NULLPTR(ffd);
    ffd->setObjectName("ffd");
    QString faceDetector = appSettings->value("Detect/FaceDetector",
                                              ffd->detectorsDefault())
                                                    .toString();
    VersionInfo cvVersion = ffd->cvVersion();
    PROGRESS("%1 %2 %3 built %4", cvVersion.getOrgName(),
             cvVersion.getAppName(), cvVersion.toString(),
             cvVersion.dateTimeString());
    INFO(cvVersion.getCopyright());

    if (faceDetector.isEmpty())
    {
        WARNING("No face detector specified in Detect/FaceDetector");
    }
    else
    {
        ffd->selectDetector(faceDetector);
        if (ffd->hasDetector())
        {
            PROGRESS("Face Detector loaded: %1 %2x%3", ffd->detectorName(),
                     ffd->detectorSize().width(), ffd->detectorSize().height());
            INFO("Description: %1", ffd->detectorDescription());
        }
        else
        {
            ERROR("Unable to load face detector: %1", ffd->detectorName());
            QTimer::singleShot(0, this, SLOT(done()));
            return DDT::Return::False;
        }
    }
    qsl.clear(); qsl << "MaxAcross" << "MinAcross" << "MinPixels" << "MaxDetectors"
                     << "MaxPixels" << "MaxResults" << "MinQuality" << "MaxDensity"
                     << "MarkColors" << "MarkScaleColor" << "MarkExtraColor" << "MarkAll"
                     << "Factor" << "GroupMethod" << "ForceFind" << "Scale" << "Flags"
                     << "MaxDimension";
    appSettings->objectProperties(ffd, tr("Detect", "config"),
                                  qsl, DDT::Settings::Volatile);
    ffd->setDefaultGroupMethod(INDI::HaarDetector::GroupByNeighbors);
    connect(ffd, SIGNAL(error(QString)), this, SLOT(error(QString)));
    ffdBusy = false;
    TRACE("FFD is free");

    PROGRESS("Initializing INDIface Data and Parameters");
    eigenData = new INDI::EigenFaceData("FaceFrontal");
    QDir dataDir(qApp->applicationDirPath());
    dataDir.cd(eigenFaceDataDir);
    INFO("Creating INDIface data from " + dataDir.absolutePath());
    DDT::Info::flush();

    rtn = eigenData->create(dataDir);
    RETURN(rtn);
    if (rtn.isError())
        return rtn;

    eigenParms = INDI::EigenFace::parameters(QDir(/*eigenFaceParmDir*/));
    if ( ! eigenParms)
    {
        rtn = INDI::EigenFace::lastReturn();
        RETURN(rtn);
        return rtn;
    }

    PROGRESS("Initializing INDIface Template Generators");
    eigenFace = new INDI::EigenFaceGenerator(this, eigenData, eigenParms);	NULLPTR(eigenFace);
    eigenFace->setObjectName("eigenFace");
    eigenFace->setFactor(optGenerateFactor->toFloat());
    qsl.clear(); qsl << "MinConsistency" << "RoiScale" << "EyeScale";
    appSettings->objectProperties(eigenFace, tr("Generate", "config"), qsl, DDT::Settings::Volatile);
    qsl.clear(); qsl << "LeftDetector" << "RightDetector";
    appSettings->objectProperties(eigenFace, tr("Generate", "config"), qsl);
    connect(eigenFace, SIGNAL(error(QString)), this, SLOT(error(QString)));

    PROGRESS("Initializing INDIface Matcher");
    eigenMatcher = new EigenFaceSearcher();
    NULLPTR(eigenMatcher);
    appSettings->objectProperties(&matchSettings, tr("Match", "config"), matchSettings.dynamicPropertyNames(), DDT::Settings::Volatile);
    appSettings->objectProperties(&searchSettings, tr("Search", "config"), searchSettings.dynamicPropertyNames(), DDT::Settings::Volatile);

    return DDT::Return();
} // initEigenFace()
