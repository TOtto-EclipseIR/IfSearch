#include "MainWindow.h"

#include <QtSvg>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QStackedLayout>
#include <QtGui/QStatusBar>

#include <eirBase/eirBase.h>
#include <eirBase/VersionInfo.h>
#include <eirCore/eirCore.h>
//#include <eirCore/SettingsVariableGroup.h>
#include <eirCore/VariableTable.h>
#include <eirCore/VariableSettingsList.h>
#include <eirExe/GuiApplication.h>
#include <eirExe/Log.h>
#include <eirExe/OptionSet.h>
#include <eirFile/TemporaryDir.h>
#include <eirGUI/IOWidget.h>
#include <eirGUI/TimeoutBar.h>
#include <eirSDK1/ImageDirSink.h>
#include <eirSDK1/PersonReportRow.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>
#include <eirSDK1/SdkClientParameters.h>
#include <eirSDK1/SdkMachine.h>

#include "IdPane.h"
#include "LivePage.h"
#include "MaintenancePage.h"
#include "StateMachine.h"
#include "version.h"
#include "../../../common/version.h"

void MainWindow::start(void)
{
    FUNCTION();
    SETVERSION();
    initializeOptions();

    opts()->setDefault("GatherDf/Options/UpdateMsec", 500);
    opts()->setDefault("GatherDf/Input/URL", "http://demo:demo@10.0.0.90//jpg/image.jpg");
    opts()->setDefault("GatherDf/Input/SampleMsec", 250);
    opts()->setDefault("GatherDf/Input/Pause", false);
    opts()->setDefault("GatherDf/Output/BaseDir", "../TPIRtemp");
    opts()->setDefault("GatherDf/Output/MarkedDir", "Marked");
    opts()->setDefault("GatherDf/Output/MarkedFaceDir", "MarkedFace");
    opts()->setDefault("GatherDf/Output/LogFile", "./log/GatherIf-@.log");
    opts()->setDefault("GatherDf/Output/ForceMarked", true);
    opts()->setDefault("GatherDf/Mark/BodyColor", QString());
    opts()->setDefault("GatherDf/Detect/Enable", true);
    opts()->setDefault("GatherDf/Detect/MaxResults", 1);
    opts()->setDefault("GatherDf/Detect/MinAcross", 0);
    opts()->setDefault("GatherDf/Detect/MaxAcross", 3);
    opts()->setDefault("GatherDf/Detect/MaxDensity", 1.0);
    opts()->setDefault("GatherDf/Detect/MaxDetectors", 20);
    opts()->setDefault("GatherDf/Detect/MaxDimension", 480);
    opts()->setDefault("GatherDf/Detect/MinQuality", 500);
    opts()->setDefault("GatherDf/Generate/Enable", 1);
    opts()->setDefault("GatherDf/Generate/LeftDetector", "LEye");
    opts()->setDefault("GatherDf/Generate/RightDetector", "REye");
    opts()->setDefault("GatherDf/Generate/MinConsistency", 400);

    opts()->setDefault("SearchDf/Options/UpdateMsec", 250);
    opts()->setDefault("SearchDf/Output/BaseDir", "../TPIRtemp");
    opts()->setDefault("SearchDf/Output/LogFile", "./log/SearchIf-@.log");
    opts()->setDefault("SearchDf/Input/Pause", true);
    opts()->setDefault("SearchDf/Search/InputDir", "SearchIn");
    opts()->setDefault("SearchDf/Search/PersonMode", true);
    opts()->setDefault("SearchDf/Search/MinConfidence", 400);

    opts()->setDefault("log/troll/Device", "Troll");
    opts()->setDefault("log/troll/Options", "LevelChar TimeOfDay");
    opts()->setDefault("log/troll/Filter/999/Report", "~");
    opts()->setDefault("log/trace/Device", "TxtFile");
    opts()->setDefault("log/trace/FileName", "../TPIRtemp/log/TPIR-@.log");
    opts()->setDefault("log/trace/Options", "LevelChar TimeOfDay");
    opts()->setDefault("log/trace/Filter/999/Report", "~");

    initializeLogger();

    QTimer::singleShot(10, this, SLOT(setupUi()));
}

void MainWindow::setupUi(void)
{
    FUNCTION();
    OBJPOINTER(IdPane, _idPane);
    OBJPOINTER(IOWidget, gather_iow);
    OBJPOINTER(IOWidget, search_iow);
    OBJPOINTER(QStackedLayout, _stack);
    OBJPOINTER(LivePage, livePage_wgt);
    OBJPOINTER(MaintenancePage, maintPage_wgt);

    QRect screen_rect = QApplication::desktop()->screenGeometry(this);
    DUMPVAR(screen_rect);

    setWindowTitle("EclipseIR TokenPointIR " + version().toString());

    QLabel * bigLogo = new QLabel;
    OBJPOINTER(QLabel, bigLogo);
    bigLogo->setPixmap(QPixmap::fromImage(QImage(":/icon/TokenPoint.svg")
                                                 .scaledToHeight(300)));
    QVBoxLayout * main_layout = new QVBoxLayout;
    iow_layout = new QBoxLayout(QBoxLayout::LeftToRight);
    OBJPOINTER(QVBoxLayout, main_layout);
    OBJPOINTER(QBoxLayout, iow_layout);
    main_layout->setObjectName("main_layout");
    main_layout->addWidget(_idPane);
    _stack->addWidget(bigLogo);
    _stack->addWidget(livePage_wgt);
    _stack->addWidget(maintPage_wgt);
    main_layout->addLayout(_stack);

    QWidget * main_widget = new QWidget();
    OBJPOINTER(QWidget, main_widget);
    main_widget->setObjectName("main_widget");
    main_widget->setLayout(main_layout);

    QWidget * iow_widget = new QWidget();
    OBJPOINTER(QWidget, iow_widget);
    iow_layout->setObjectName("iow_layout");
    iow_layout->addWidget(gather_iow);
    iow_layout->addWidget(search_iow);
    iow_widget->setLayout(iow_layout);

    QSplitter * main_splitter = new QSplitter(Qt::Vertical);
    main_splitter->addWidget(main_widget);
    main_splitter->addWidget(iow_widget);
    setCentralWidget(main_splitter);

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QTimer::singleShot(10, this, SLOT(setupStatusBar()));
}

void MainWindow::setupStatusBar(void)
{
    FUNCTION();
    QStatusBar * sts = statusBar();
    OBJPOINTER(QStatusBar, sts);
    OBJPOINTER(QLabel, mainState_statusLabel);
    OBJPOINTER(QLabel, gatherState_statusLabel);
    OBJPOINTER(QLabel, searchState_statusLabel);
    sts->setObjectName("StatusBar");
    mainState_statusLabel->setObjectName("mainState_statusLabel");
    gatherState_statusLabel->setObjectName("gatherState_statusLabel");
    searchState_statusLabel->setObjectName("searchState_statusLabel");
    mainState_statusLabel->setText("Main State");
    gatherState_statusLabel->setText("Gather State");
    searchState_statusLabel->setText("Search State");
    sts->addPermanentWidget(gather_sdk->machine()->timeoutBar(sts));
    sts->addPermanentWidget(search_sdk->machine()->timeoutBar(sts));
    sts->addPermanentWidget(mainState_statusLabel);
    sts->addPermanentWidget(gatherState_statusLabel);
    sts->addPermanentWidget(searchState_statusLabel);
    CONNECT(this, SIGNAL(stateName(QString)),
            mainState_statusLabel, SLOT(setText(QString)));
    CONNECT(live(), SIGNAL(stateName(QString)),
            mainState_statusLabel, SLOT(setText(QString)));
    CONNECT(maint(), SIGNAL(stateName(QString)),
            mainState_statusLabel, SLOT(setText(QString)));
    CONNECT(gather_sdk, SIGNAL(stateName(QString)),
            gatherState_statusLabel, SLOT(setText(QString)));
    CONNECT(search_sdk, SIGNAL(stateName(QString)),
            searchState_statusLabel, SLOT(setText(QString)));
    CONNECT(this, SIGNAL(stateName(QString)),
            sts, SLOT(update()));
    CONNECT(live(), SIGNAL(stateName(QString)),
            sts, SLOT(update()));
    CONNECT(maint(), SIGNAL(stateName(QString)),
            sts, SLOT(update()));
    CONNECT(this, SIGNAL(stateName(QString)),
            sts, SLOT(update()));
    CONNECT(gather_sdk, SIGNAL(stateName(QString)),
            sts, SLOT(update()));
    CONNECT(search_sdk, SIGNAL(stateName(QString)),
            sts, SLOT(update()));

    QTimer::singleShot(10, this, SLOT(initialize()));
}

void MainWindow::initialize(void)
{
    FUNCTION();
    OBJPOINTER(SdkClient, gather_sdk);
    OBJPOINTER(SdkClient, search_sdk);

#ifdef QT_NO_DEBUG
    QDir exeDir("../../INDIface/bin-w32");
#else
    QDir exeDir("../../common/dbg");
#endif
    VariableSet gatherDefault_vset = opts()->exportSection("GatherDf");
    VariableSet searchDefault_vset = opts()->exportSection("SearchDf");
    VariableSettings * gather_settings
            = new VariableSettings("EclipseIR", "TokenAuth-GatherIf", this);
    VariableSettings * search_settings
            = new VariableSettings("EclipseIR", "TokenAuth-SearchIf", this);
    OBJPOINTER(VariableSettings, gather_settings);
    OBJPOINTER(VariableSettings, search_settings);

    gather_settings->setObjectName("gather_settings");
    search_settings->setObjectName("search_settings");
    gatherDefault_vset.setName("gather_settings");
    searchDefault_vset.setName("search_settings");
    gather_sdk->machine()->setObjectName("gather_machine");
    search_sdk->machine()->setObjectName("search_machine");

    LOG(dump(gatherDefault_vset));
    LOG(dump(searchDefault_vset));
    LOG(dump(*gather_settings));
    LOG(dump(*search_settings));
    gather_sdk->setInterface(gather_settings, gatherDefault_vset);
    search_sdk->setInterface(search_settings, searchDefault_vset);
    LOG(dump(*gather_sdk->interface()));
    LOG(dump(*search_sdk->interface()));

    gather_parms.setBinary_PathName(exeDir.absolutePath());
    search_parms.setBinary_PathName(exeDir.absolutePath());
    gather_sdk->setResumeUrl(gather_sdk->interface()
                                       ->value("Input/URL").toString());

    CONNECT(gather_sdk, SIGNAL(initialized()),
            this, SLOT(started()));
    CONNECT(search_sdk, SIGNAL(initialized()),
            this, SLOT(started()));
    CONNECT(this, SIGNAL(shiftPersonId(QString)),
            this, SLOT(personVideoKey(QString)));
    qsrand((uint)MillisecondTime::base());
    _machine->initialize();
    _machine->start();
}

void MainWindow::initError(void)
{
    emit initializeError();
}

void MainWindow::started(void)
{
    FUNCTION();
    TRACE("from: %1", sender()->objectName());
    DISCONNECT(sender(), SIGNAL(initialized()), this, SLOT(started()));
    if (sender() == search_sdk)
    {
        QDir dir;
        SdkCommand command = search_sdk->newCommand("Retrieve", "PersonReport");
        QString csvDirName = search_sdk->dirName("Retrieve/OutputDir");
        if (csvDirName.isEmpty())
        {
            TemporaryDir temp(QDir::temp(), "Report", false);
            dir = temp;
        }
        else
        {
            dir.setPath(csvDirName);
        }
        QString csvFileName = dir.absoluteFilePath("PersonReport.csv");
        command.set("Settings/Retrieve/ReportFile", csvFileName);
        startPending_list.append(this); // for gotPersonReport()
        CONNECT(search_sdk, SIGNAL(retrieveResult(SdkCommand)),
                this, SLOT(personReportResult(SdkCommand)));
        CONNECT(this, SIGNAL(gotPersonReport()),
                this, SLOT(areAllStarted()));
        //command.dump();
        search_sdk->submit(command);
        TRACE("submit PersonReport Retrieve #%1", command.key());
    }
    areAllStarted();
}

void MainWindow::areAllStarted(void)
{
    startPending_list.removeOne(QObject::sender());
    if (startPending_list.isEmpty())
    {
        TRACE("emit allStarted()");
        emit allStarted();
    }
}

void MainWindow::enterInitialize(void)
{
    FUNCTION();
    OBJPOINTER(IdPane, _idPane);
    OBJPOINTER(SdkClient, gather_sdk);
    OBJPOINTER(SdkClient, search_sdk);

    emit stateName("Initializing");
    _idPane->message("Initializing");

    gather_sdk->machine()->initialize();
    search_sdk->machine()->initialize();
    CONNECT(gather_sdk, SIGNAL(initialized()),
            this, SLOT(started()));
    CONNECT(search_sdk, SIGNAL(initialized()),
            this, SLOT(started()));
    CONNECT(gather_sdk, SIGNAL(error(QString)),
            this, SLOT(initError()));
    CONNECT(search_sdk, SIGNAL(error(QString)),
            this, SLOT(initError()));
    startPending_list << gather_sdk << search_sdk;

    QTimer::singleShot(10, gather_sdk->machine(), SLOT(start()));
    QTimer::singleShot(10, search_sdk->machine(), SLOT(start()));
}

void MainWindow::leaveInitialize(void)
{
    FUNCTION();
    OBJPOINTER(IdPane, _idPane);
    OBJPOINTER(SdkClient, gather_sdk);

    _idPane->reset();

    if (gather_sdk->errorString().isEmpty()
            && search_sdk->errorString().isEmpty())
    {
        marked_dir = new ImageDirSink(gather_sdk->dirName("MarkedDir"), this);
        OBJPOINTER(ImageDirSink, marked_dir);
        CONNECT(marked_dir, SIGNAL(gotImage(QImage,QString)),
                live(), SLOT(gotMarkedImage(QImage,QString)));

        markedFace_dir = new ImageDirSink(gather_sdk->dirName("MarkedFaceDir"), this);
        OBJPOINTER(ImageDirSink, markedFace_dir);
        CONNECT(markedFace_dir, SIGNAL(gotImage(QImage,QString)),
                live(), SLOT(gotMarkedFace(QImage,QString)));

        CONNECT(this, SIGNAL(keyToken(QString)),
                this, SLOT(checkToken(QString)));
        sourceKey_bool = true;
    }
}

