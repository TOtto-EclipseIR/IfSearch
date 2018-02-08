#include "MainWindow.h"

#include "version.h"
#include "../../../common/version.h"

#include <QtCore/QTemporaryFile>
#include <QtCore/QTimer>
#include <QtGui/QGridLayout>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

#include <eirExe/Log.h>
#include <eirExe/OptionSet.h>
#include <eirExe/Results.h>
#include <eirExe/ResultRegistry.h>
#include <eirGUI/eirGUI.h>
#include <eirGUI/CheckAction.h>
#include <eirGUI/QQIcon.h>
#include <eirGUI/ImageLabel.h>
#include <eirFile/QQDir.h>
#include <eirFrameSource/FrameSourceManager.h>
#include <eirFrameSource/FrameSourcePlugin.h>
#include <eirFrameSource/FrameSourceDirect.h>
#include <eirFrameSource/FrameSourceResult.h>

MainWindow::MainWindow(QWidget *parent)
    : GuiApplication(parent)
    , main_imageLabel(new ImageLabel())
    //, stopStart_action(0)
    , pauseResume_action(0)
    , abort_action(0)
    , _manager(new FrameSourceManager)
    , _plugin(0)
    , _interface(0)
    , output_dir(0)
{
    OBJPOINTER(ImageLabel, main_imageLabel);
    NULPOINTER(_manager);

    main_imageLabel->setObjectName("main_imageLabel");
    SETVERSION();

    QTimer::singleShot(10, this, SLOT(initialize()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize(void)
{
    FNSLOT();

    initializeOptions();
    initializeLogger();

    ResultRegistry::dump();

    SHOOT(100, this, SLOT(setupUi()));
}

void MainWindow::setupUi(void)
{
    FNSLOT();

    setWindowTitle("EclipseIR GrabberIR " + version().toString());

    QGridLayout * mainGrid = new QGridLayout;
    QWidget * mainWgt = new QWidget;
    OBJPOINTER(QGridLayout, mainGrid);
    OBJPOINTER(QWidget, mainWgt);
    mainGrid->setObjectName("mainGrid");
    mainWgt->setObjectName("mainWgt");
    mainWgt->setLayout(mainGrid);
    setCentralWidget(mainWgt);

    main_imageLabel->setDefault(eirGUI::eirLogo(QSize(320,320)));
    main_imageLabel->setMinimumSize(QSize(640, 480));
    mainGrid->addWidget(main_imageLabel);
/*
    stopStart_action = new CheckAction(this);
    stopStart_action->setIcon(QQIcon("action-stop"),
                                QQIcon("action-start"));
    stopStart_action->setText("Stop Source", "Restart Source");
    stopStart_action->setIconText("Stop", "Restart");
    stopStart_action->setToolTip("Stop", "Restart");
    stopStart_action->setChecked(false);
    CONNECT(stopStart_action, SIGNAL(toggled(bool)),
                this, SLOT(stopStart(bool)));
    pauseResume_action = new CheckAction(this);
    pauseResume_action->setIcon(QQIcon("media-playback-pause"),
                                QQIcon("media-playback-resume"));
    pauseResume_action->setText("Pause Video", "Resume Playing");
    pauseResume_action->setIconText("Pause", "Resume");
    pauseResume_action->setToolTip("Pause", "Resume");
    pauseResume_action->setChecked(false);
    pauseResume_action->setEnabled(false);
    CONNECT(pauseResume_action, SIGNAL(toggled(bool)),
                this, SLOT(pauseResume(bool)));

    abort_action = new QAction(QQIcon("action-abort"), "Abort", this);
    CONNECT(abort_action, SIGNAL(triggered()),
            this, SLOT(abort()));

    QToolBar * nav_toolbar = addToolBar("Navigation");
    nav_toolbar->setIconSize(QSize(48, 48));
    //nav_toolbar->addAction(stopStart_action);
    nav_toolbar->addAction(pauseResume_action);
    nav_toolbar->addAction(abort_action);
*/

    SHOOT(100, this, SLOT(initializeManager()));
}

void MainWindow::initializeManager(void)
{
    FNSLOT();
    OBJPOINTER(FrameSourceManager, _manager);
    QString dirName = opts()->value("Source/PluginDir").toString();
    _manager->initialize(dirName.isEmpty() ? QQDir()
                                           : QQDir(dirName));
    statusBar()->showMessage("Initialized");
    opts()->write("Options/Status", "Initialized");

    CONNECT(opts(), SIGNAL(triggered(QString,QVariant)),
            this, SLOT(optWatch(QString,QVariant)));
    opts()->addTrigger("Options/Command");

    if (opts()->value("Options/AutoStart").toBool())
        SHOOT(100, this, SLOT(checkConfig()));
}

void MainWindow::checkConfig(void)
{
    opts()->read();
    VariableSet config = opts()->exportSection("Source");
    config.setName("config:Source");

    QString urlString = config.value("URL").toString();
    QUrl new_url = QUrl(urlString);

    if (new_url != _url)
    {
        if (urlString.isEmpty())
        {
            if (_plugin)
                _plugin->shutdown();
            _plugin = 0;
            _url = QUrl();
        }
        else
        {
            _url = new_url;
            if ( ! _plugin)
                autoStart(config);
        }
    }

    //if (config.value("Paused").toBool())
      //  pauseResume_action->setChecked(true);

    SHOOT(1000, this, SLOT(checkConfig()));
}

void MainWindow::autoStart(const VariableSet & config)
{
    FNSLOT(config.name());
    DUMPVSET(config);
    OBJPOINTER(FrameSourceManager, _manager);

    if ( ! _url.isValid())
    {
        ERROR("Invalid URL: %1", _url.toString());
        statusBar()->showMessage("Invalid URL: "+_url.toString());
        opts()->write("Options/Status", "Invalid URL: "+_url.toString());
        return;
    }

    _plugin = _manager->plugin(_url, this);
    if ( ! _plugin)
    {
        ERROR("Unable to load plugin for %1", _url.scheme());
        statusBar()->showMessage("Unable to load plugin for "+_url.scheme());
        opts()->write("Options/Status", "Unable to load plugin for "+_url.scheme());
        return;
    }
    FSPPOINTER(FrameSourcePlugin, _plugin);

    channel_i = config.value("Channel").toInt();
    beginTime_s = config.value("Begin").toString();
    endTime_s = config.value("End").toString();

    _interface = new FrameSourceDirect(_plugin);
    OBJPOINTER(FrameSourceDirect, _interface);
    CONNECT(_interface, SIGNAL(connected(ResultSet)),
            this, SLOT(connected(ResultSet)));
    CONNECT(_interface, SIGNAL(configured(ResultSet)),
            this, SLOT(configured(ResultSet)));
    CONNECT(_interface, SIGNAL(started(ResultSet)),
            this, SLOT(started(ResultSet)));
    CONNECT(_interface, SIGNAL(grabbed(int)),
            this, SLOT(takeGrab(int)));
    CONNECT(_interface, SIGNAL(complete(ResultSet)),
            this, SLOT(complete(ResultSet)));
    CONNECT(_interface, SIGNAL(warning(Result)),
            this, SLOT(warning(Result)));
    CONNECT(_interface, SIGNAL(messageImage(ImageEntity)),
            this, SLOT(showImage(ImageEntity)));

    _interface->autoStart(_url,
                          config,
                          channel_i,
                          beginTime_s,
                          endTime_s);

    QString baseDirname(opts()->value("Output/BaseDir").toString());
    QString captureDirname(opts()->value("Output/CaptureDir").toString());
    DUMPVAR(baseDirname);
    DUMPVAR(captureDirname);
    if ( ! captureDirname.isEmpty())
    {
        output_dir = new QQDir(baseDirname);
        NULPOINTER(output_dir);
        if ( ! output_dir->exists(captureDirname))
            EXPECT(output_dir->mkpath(captureDirname));
        if ( ! output_dir->cd(captureDirname))
        {
            delete output_dir;
            output_dir = 0;
        }
        else
        {
            DUMPVAR(output_dir->absolutePath());
        }
    }
}

void MainWindow::connected(ResultSet rset)
{
    FNSLOT();
    RESULTS(rset);
    statusBar()->showMessage("Connected");
    opts()->write("Options/Status", "Connected");
}

void MainWindow::configured(ResultSet rset)
{
    FNSLOT();
    RESULTS(rset);
}

void MainWindow::started(ResultSet rset)
{
    FNSLOT();
    RESULTS(rset);
    statusBar()->showMessage("Started");
    opts()->write("Options/Status", "Started");
    //pauseResume_action->setEnabled(true);
}

void MainWindow::complete(ResultSet rset)
{
    FNSLOT(rset.toStringList());
    RESULTS(rset);
    Result cRes = rset.find(FrameSourceResult::PlaybackComplete);
    if ( ! cRes.isNull())
    {
        int nFrames = cRes.value("NumFrames").toInt();
        if (nFrames)
            MainWindow::statusBar()
                    ->showMessage(QString("%1 frames complete").arg(nFrames));
        else
            MainWindow::statusBar()->showMessage("Complete");
        opts()->write("Options/Status", "Complete");
    }
}

void MainWindow::takeGrab(int key)
{
    FNSLOT(key);
    OBJPOINTER(FrameSourceInterfaceBase, _interface);
    ImageEntity frameEntity(_interface->take(key));
    if (EXPECTNOT(frameEntity.isNull()))
        return;
#if 0
    QString frameId(frameEntity.id());
    QByteArray frameBA(frameEntity.binaryData());
    QImage frameImage;

    if (output_dir)
    {
        QFile frameFile(output_dir->absoluteFilePath(frameId+".tmp"));
        EXPECT(frameFile.open(QFile::WriteOnly));
        EXPECT(frameFile.write(frameBA));
        frameFile.close();
        EXPECT(frameFile.rename(output_dir->absoluteFilePath(frameId+".jpg")));
        EXPECT(frameImage.load(output_dir->absoluteFilePath(frameId+".jpg"), "jpg"));
    }
    else
    {
        QTemporaryFile tempFile("Frame.jpg");
        EXPECT(tempFile.open());
        EXPECT(tempFile.write(frameBA));
        tempFile.close();
        EXPECT(frameImage.load(tempFile.fileName(), "jpg"));
    }
    DUMPHEX(frameBA.constData(), 256)

    main_imageLabel->set(frameImage);
    if ( ! frameId.isEmpty())
        MainWindow::statusBar()->showMessage(frameId);
#else
    QImage frameImage(frameEntity.toQImage());
    QString frameId(frameEntity.id());
    if (frameImage.isNull())
    {
        frameImage = QImage(640, 480, QImage::Format_ARGB32_Premultiplied);
        frameImage.fill(Qt::green);
        frameId = "Null Image " + QString::number(key % 60000);
    }

    main_imageLabel->set(frameImage);
    if ( ! frameId.isEmpty())
        MainWindow::statusBar()->showMessage(frameId);

    if (output_dir && ! frameId.startsWith("Null Image"))
    {
        QByteArray frameBA(frameEntity.binaryData());
        QFile frameFile(output_dir->absoluteFilePath(frameId+".tmp"));
        EXPECT(frameFile.open(QFile::WriteOnly));
        EXPECT(frameFile.write(frameBA));
        frameFile.close();
        EXPECT(frameFile.rename(output_dir->absoluteFilePath(frameId+".jpg")));
    }
#endif
}

void MainWindow::stopStart(bool checked)
{
    FUNCTION(checked);
    if ( ! _interface) return;
    OBJPOINTER(FrameSourceInterfaceBase, _interface);

    if (checked)
        _interface->stop();
    else
        _interface->start(channel_i, beginTime_s, endTime_s);
}

void MainWindow::pauseResume(bool checked)
{
    FUNCTION(checked);
    if ( ! _interface) return;
    OBJPOINTER(FrameSourceInterfaceBase, _interface);

    if (checked)
        _interface->pause();
    else
        _interface->resume();
}

void MainWindow::abort(void)
{
    FUNCTION();
    if ( ! _interface) return;
    OBJPOINTER(FrameSourceInterfaceBase, _interface);

    _interface->abort();
}

void MainWindow::optWatch(QString vid,
                          QVariant var)
{
    FUNCTION(vid, var);

    if ("Options/Command" != vid) return;

    QString command(var.toString());
    if (0 == command.compare("Connect", Qt::CaseInsensitive))
    {
        SHOOT(50, this, SLOT(connectUrl()));
    }
    else if (0 == command.compare("AutoStart", Qt::CaseInsensitive))
    {
        if (_interface)
        {
            WARNING("AutoStart with existing interface");
        }
        else
        {
            opts()->read();
            VariableSet config = opts()->exportSection("Source");
            _url = QUrl(config.value("URL").toString());
            autoStart(config);
        }
    }
    else if (0 == command.compare("AutoStop", Qt::CaseInsensitive))
    {
        SHOOT(50, this, SLOT(autoStop()));
        statusBar()->showMessage("Disconnected");
        opts()->write("Options/Status", "Disconnected");
    }
    else if (0 == command.compare("Pause", Qt::CaseInsensitive))
    {
        if (_interface)
        {
            OBJPOINTER(FrameSourceDirect, _interface);
            _interface->pause();
        }
        else
            WARNING("Pause without instance");
    }
    else if (0 == command.compare("Resume", Qt::CaseInsensitive))
    {
        if (_interface)
        {
            OBJPOINTER(FrameSourceDirect, _interface);
            _interface->resume();
        }
        else
            WARNING("Resume without instance");
    }
    else
        WARNING("Unhandled command=%1", command);
}

void MainWindow::showImage(ImageEntity ie)
{
    QImage frameImage(ie.toQImage());
    main_imageLabel->set(frameImage);
    QString title(ie.value("Message/Title").toString());
    statusBar()->showMessage(title);
    opts()->write("Options/Status", title);

    if (output_dir)
    {
        QString id(ie.value("Message/Severity").toString());
        if (EXPECT(frameImage.save(output_dir->absoluteFilePath(id+".jpg"), "jpg")))
            TRACE("Wrote %1", output_dir->absoluteFilePath(id+".jpg"))
    }
}

void MainWindow::autoStop(void)
{
    if (qobject_cast<FrameSourceDirect *>(_interface))
        delete _interface;
    _interface = 0;
    if (qobject_cast<FrameSourcePlugin *>(_plugin))
        delete _plugin;
    _plugin = 0;
    _url.clear();
}

void MainWindow::warning(Result res)
{
    FUNCTION(res.toString());
    QString s(QString("%1: %2").arg(res.severity().name())
                               .arg(res.toString()));
    statusBar()->showMessage(s);
    opts()->write("Options/Status", s);
    if (res.severity().isError())
        SHOOT(50, this, SLOT(autoStop()));
}

void MainWindow::connectUrl(void)
{
    FUNCTION();

    QString urlString = opts()->value("Source/URL").toString();
    QUrl new_url = QUrl(urlString);
    if ( ! new_url.isValid())
    {
        statusBar()->showMessage("Invalid URL: " + urlString);
        return;
    }

    _url = new_url;
    _plugin = _manager->plugin(_url, this);
    if ( ! _plugin)
    {
        statusBar()->showMessage("Unable to load plugin for "+_url.toString());
        return;
    }
    FSPPOINTER(FrameSourcePlugin, _plugin);

    _interface = new FrameSourceDirect(_plugin);
    OBJPOINTER(FrameSourceDirect, _interface);
    CONNECT(_interface, SIGNAL(connected(ResultSet)),
            this, SLOT(connected(ResultSet)));
    CONNECT(_interface, SIGNAL(configured(ResultSet)),
            this, SLOT(configured(ResultSet)));
    CONNECT(_interface, SIGNAL(started(ResultSet)),
            this, SLOT(started(ResultSet)));
    CONNECT(_interface, SIGNAL(grabbed(int)),
            this, SLOT(takeGrab(int)));
    CONNECT(_interface, SIGNAL(complete(ResultSet)),
            this, SLOT(complete(ResultSet)));
    _interface->connect();
}
