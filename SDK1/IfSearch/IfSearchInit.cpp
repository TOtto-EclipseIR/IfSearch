#include "IfSearch.h"

#include <QLibraryInfo>
#include <QProcessEnvironment>
#include <QTimer>

#include <ClothesMatchProperties.h>
#include <DDTcore.h>
#include <EigenFace.h>
#include <EigenFaceData.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceSearcher.h>
#include <EigenFaceSearchTier.h>
#include <EigenFaceSimilarity.h>
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
#include <Resolver.h>
#include <ClothesMatchProperties.h>
#include <HeightGrid.h>
#include <SkinMatcher.h>
#include <SkinMatchProperties.h>
#ifdef ENABLE_WATCHDOG
#include <eirExe/WatchDog.h>
#endif

/*
typedef struct _PROCESS_INFORMATION {
  void * hProcess;
  void * hThread;
  quint32  dwProcessId;
  quint32  dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
*/

void IfSearch::init(void)
{
    Return rtn;
    QStringList qsl;

    appSettings->setPollCountKey("Options/PollCount");
    if (appSettings->value("Output/LogStdout", true).toBool())
        Info::add(new InfoOutputFile(stdout));

    QString sDetail = optLogDetail->toString();
    InfoSeverity logDetail = InfoSeverity::Info;
    if (0 == sDetail.compare("Null", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Null;
    else if (0 == sDetail.compare("Leave", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Leave;
    else if (0 == sDetail.compare("Enter", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Enter;
    else if (0 == sDetail.compare("Detail", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Detail;
    else if (0 == sDetail.compare("Debug", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Debug;
    else if (0 == sDetail.compare("Info", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Info;
    else if (0 == sDetail.compare("Progress", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Progress;
    else if (0 == sDetail.compare("Warning", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Warning;
    else if (0 == sDetail.compare("Error", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Error;
    else if (0 == sDetail.compare("Fatal", Qt::CaseInsensitive))
        logDetail = InfoSeverity::Fatal;
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
        Info::add(new InfoOutputFile(logFileName),
                       logDetail.isValid() ? logDetail : InfoSeverity::Info);
    }

    Info::add(infoSetting, InfoSeverity::Warning,
                                InfoSeverity::Fatal);
    Info::start(QThread::NormalPriority);
//    PROGRESS("%3 %1 built %2", version.toString(),
  //           version.dateTimeString(), version.getAppName());
    INFO(version.getCopyright());
    INFO(appSettings->programName());
    DETAIL("Running ProcessId=%1", applicationPid());
    version.check(0xE3ECE9F0);
    if ( ! initLicense())
    {
         QTimer::singleShot(10000, this, SLOT(done()));
         return;
    }

    qreal rolloverHours = appSettings->value("Output/RolloverHours").toReal();
    int rolloverKeep = appSettings->value("Output/RolloverKeep").toInt();
    if ( ! qIsNull(rolloverHours))
    {
        Info::setRolloverKeep(rolloverKeep);
        MillisecondDelta msd;
        msd.setHours(rolloverHours);
        rolloverTimer = new QTimer(this);
        rolloverTimer->setInterval(msd.msec());
        if (connect(rolloverTimer, SIGNAL(timeout()), this, SLOT(logRollover())))
        {
            INFO("Log Rollover started for %1 hours keeping %2",
                 rolloverHours, rolloverKeep);
            rolloverTimer->start();
        }
        else
        {
            ERRMSG("Error CONNECTing logRollover() slot");
        }
    }

    PROGRESS("Running Qt Version %1", qVersion());
    detectorsXml     = appSettings->value("Detect/DetectorsXml", "../detectors/Detectors.xml").toString();
    eigenFaceDataDir = appSettings->value("Generate/DataDir", "../data/Face1").toString();
    faceBaseBaseDir  = appSettings->value("FaceBase/BaseDir", "../FaceBase").toString();
    faceBaseMaxLoad  = appSettings->value("FaceBase/MaxLoad", 50).toInt();
    faceBaseReload_msd.setToMinutes(appSettings->value("FaceBase/ReloadMins", 0.0).toFloat());

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
    Return rtn;
    QStringList qsl;

    /*--- Setup Clothes Matcher ---*/
//    appSettings->objectProperties(clothesMatchProperties, tr("Clothes", "config"), clothesMatchProperties->dynamicPropertyNames(), Settings::Volatile);

    /*--- Initialize Height Grid ---*/
    heightGrid = new HeightGrid(this);
    NULLPTR(heightGrid);
    connect(heightGrid, SIGNAL(error(QString)), this, SLOT(error(QString)));
    appSettings->objectProperties(heightGrid, tr("Height", "config"), heightGrid->dynamicPropertyNames(), Settings::Volatile);

    /*--- Setup Skin Matchers ---*/
    appSettings->beginGroup("FaceColor");
    qsl = appSettings->childGroups();
    appSettings->endGroup();
    foreach (QString name, qsl)
    {
        SkinMatchProperties * smp = new SkinMatchProperties(name, this);
        appSettings->objectProperties(smp, "FaceColor/" + name, DDTcore::propertyNames(smp), Settings::Volatile);
        FileWriteProfile * fwpOutput = writer->newProfile(name+"-Output", FileWriter::FaceImage, "FaceColor/"+name+"/OutputDir");
        FileWriteProfile * fwpMarked = writer->newProfile(name+"-Marked", FileWriter::$nullFlag, "FaceColor/"+name+"/MarkedDir");
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
    appSettings->objectProperties(&xformProps, tr("PreProcess", "config"), xformProps.dynamicPropertyNames(), Settings::Volatile);

    /*--- Setup Resolver ---*/
    resolver = new Resolver(this);  NULLPTR(resolver);
    qsl.clear(); qsl << "Quality" << "Consistency" << "FaceColor"
                     << "UpperClothes" << "LowerClothes" << "Height";
    foreach (IdString name, qsl)
    {
        Setting * opt = new Setting(appSettings, "Resolve/" + name, 0, Settings::Volatile);
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
            ERRMSG("Unable to write FramePerformanceFile %1: %2", fpFileName, fpWriter->errorString());
            delete fpWriter;
            fpWriter = 0;
        }

    }


    appSettings->objectProperty(appSettings,
                                tr("Options", "config"),
                                tr("UpdateMsec", "config"),
                                Settings::Volatile);
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
    if ( ! check(368))
        return;

    rtn = initEigenFace();
    RETURN(rtn);
    if (rtn.isError() || rtn.isFalse())
    {
        QTimer::singleShot(0, this, SLOT(done()));
        return;
    }

    PROGRESS("Initializing INDI FaceBase");
    faceBase = new FaceBase(this, eigenMatcher);	NULLPTR(faceBase);
    INFO("from %1", faceBaseBaseDir);
    rtn = faceBase->initialize(faceBaseBaseDir, faceBaseMaxLoad);
    RETURN(rtn);
    if (rtn.isError() || rtn.isFalse())
    {
        QTimer::singleShot(0, this, SLOT(done()));
        return;
    }

    if ( ! faceBaseReload_msd.isNull())
    {
        reloadTimer = new QTimer(this);
        reloadTimer->setInterval(faceBaseReload_msd.msec());
        connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reload()));
        reloadTimer->start();
    }
#ifdef ENABLE_WATCHDOG
    MillisecondDelta interval;
    interval.setToMinutes(appSettings->value("Options/WatchDogMins", "0.0").toFloat());
    if ( ! interval.isNull())
    {
        _watchdog->setInterval(interval);
        connect(_watchdog, SIGNAL(bite(QVariant,MillisecondTime)),
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
    appSettings->dump(InfoSeverity::Info);

    writer->start();
    writer->pumpAll();
    writer->dump();

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

#if 0
#define DLL
#include "C:/CrypKey SDK/Build 7723/Include/crypkey.h"

#define MASTER_KEY \
    "6a6167abf35cb0253b91761971cfcecfe79b6935d6e77905f5b29a45badd3a4f"  \
    "21315cee18798f96ef1861455c0f18643746f1183580caa6a3f5ef6d8e3dd7e15" \
    "da0f3ab21414b8fbe549d385552f816406b15040070d636ab2153d715e71c6c2e" \
    "eda08f58503f64ce9183759075949ff249013d045b55907aa6924c5d707546"

#define USER_KEY "D050 815C D1A2 A79D B1"

#define CHECKPERIOD 15
#endif

bool IfSearch::initLicense(void) const
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    DETAIL("System Environment:");
    foreach(QString var, env.toStringList())
        DETAIL(var);
#if 0
    PROGRESS("Initializing SDK1 License");

    int code = -85;
    unsigned long level = 0;

    // --- Try to initialize Crypkey
    QString phase("Init");
    QString path(appSettings->programName());
#if 1 // Crypkey trial period
    QFileInfo fi(path);
    fi.setFile("EXAMPLE.EXE");
    path = fi.absoluteFilePath();
#endif
    char bstr[MAX_PATH];
    qstrcpy(bstr, qPrintable(QDir::toNativeSeparators(path)));
    code = InitCrypkey(bstr,
                       MASTER_KEY,
                       USER_KEY,
                       FALSE, /* No Floppy License */
                       3 * CHECKPERIOD);

    // --- Try Crypkey Authorization
    if (INIT_OK == code)
    {
        code = GetAuthorization(&level, 0);

    }


    // --- We're done one way or the other
    if (code)
    {
        ERRMSG("Licensing Failed; code=%1 during %2", code, phase);
        INFO("Please contact EclipseIR to resolve at");
        INFO("mailto:matchpoint_prerelease@eclipseir.com");
        return false;
    }
    else
    {
        PROGRESS("Licencing Complete");
        return true;
    }
#else
    return true;
#endif
}

Return IfSearch::initEigenFace(void)
{
    QStringList qsl;
    Return rtn;

    QDir xmlDir(qApp->applicationDirPath());
    QFile file(xmlDir.absoluteFilePath(detectorsXml));
    INFO("Reading detectors from " + file.fileName());
    rtn = HaarDetector::initialize(&file);
    RETURN(rtn);
    if (rtn.isError())
        return rtn;

    PROGRESS("Initializing INDIface Face Detector");
//    unsigned faceInterface = appSettings->value("Detect/Interface", 0).toUInt();
    ffd = new FrontalFaceDetector(this); NULLPTR(ffd);
    ffd->setObjectName("ffd");
    QString faceDetector = appSettings->value("Detect/FaceDetector",
                                              ffd->detectorsDefault())
                                                    .toString();
#if 0
    VersionInfo cvVersion = ffd->cvVersion();
    PROGRESS("%1 %2 %3 built %4", cvVersion.getOrgName(),
             cvVersion.getAppName(), cvVersion.toString(),
             cvVersion.dateTimeString());
    INFO(cvVersion.getCopyright());
#endif
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
            ERRMSG("Unable to load face detector: %1", ffd->detectorName());
            QTimer::singleShot(0, this, SLOT(done()));
            return Return::False;
        }
    }
    qsl.clear(); qsl << "MaxAcross" << "MinAcross" << "MinPixels" << "MaxDetectors"
                     << "MaxPixels" << "MaxResults" << "MinQuality" << "MaxDensity"
                     << "MarkColors" << "MarkScaleColor" << "MarkExtraColor" << "MarkAll"
                     << "Factor" << "GroupMethod" << "ForceFind" << "Scale" << "Flags"
                     << "MaxDimension";
    appSettings->objectProperties(ffd, tr("Detect", "config"),
                                  qsl, Settings::Volatile);
    ffd->setDefaultGroupMethod(HaarDetector::GroupByNeighbors);
    connect(ffd, SIGNAL(error(QString)), this, SLOT(error(QString)));
    ffdBusy = false;
    TRACE("FFD is free");

    PROGRESS("Initializing INDIface Data and Parameters");
    eigenData = new EigenFaceData("FaceFrontal");
    QDir dataDir(qApp->applicationDirPath());
    dataDir.cd(eigenFaceDataDir);
    INFO("Creating INDIface data from " + dataDir.absolutePath());
    Info::flush();

    rtn = eigenData->create(dataDir);
    RETURN(rtn);
    if (rtn.isError())
        return rtn;

    eigenParms = EigenFace::parameters(QDir(/*eigenFaceParmDir*/));
    if ( ! eigenParms)
    {
        rtn = EigenFace::lastReturn();
        RETURN(rtn);
        return rtn;
    }

    PROGRESS("Initializing INDIface Template Generators");
    eigenFace = new EigenFaceGenerator(this, eigenData, eigenParms);	NULLPTR(eigenFace);
    eigenFace->setObjectName("eigenFace");
    eigenFace->setFactor(optGenerateFactor->toFloat());
    qsl.clear(); qsl << "MinConsistency" << "RoiScale" << "EyeScale";
    appSettings->objectProperties(eigenFace, tr("Generate", "config"), qsl, Settings::Volatile);
    qsl.clear(); qsl << "LeftDetector" << "RightDetector";
    appSettings->objectProperties(eigenFace, tr("Generate", "config"), qsl);
    connect(eigenFace, SIGNAL(error(QString)), this, SLOT(error(QString)));

    PROGRESS("Initializing INDIface Matcher");
    eigenMatcher = new EigenFaceSearcher();
    NULLPTR(eigenMatcher);
    appSettings->objectProperties(&matchSettings, tr("Match", "config"), matchSettings.dynamicPropertyNames(), Settings::Volatile);
    appSettings->objectProperties(&searchSettings, tr("Search", "config"), searchSettings.dynamicPropertyNames(), Settings::Volatile);
    searchSimilarity = new EigenFaceSimilarity(&searchSettings);

    return Return();
} // initEigenFace()
