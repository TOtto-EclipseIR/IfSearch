#include "FSDirectBridge.h"

#include <QtCore/QTimer>

#include <eirExe/Log.h>
#include <eirExe/Results.h>
#include <eirImage/ImageEntity.h>
#include <eirFrameSource/FrameSourceManager.h>
#include <eirFrameSource/FrameSourcePlugin.h>
#include <eirFrameSource/FrameSourceDirect.h>

#include "../IfSearch/ImageCache.h"
bool paused_b;
int maxCache_i;

FSDirectBridge::FSDirectBridge(DDT::ImageCache * parent)
    : QObject(parent)
    , _cache(parent)
    , _manager(new FrameSourceManager(this))
    , _interface(0)
    , resume_timer(0)
{
    FUNCTION(OBJNAME(parent));
    OBJPOINTER(FrameSourceManager, _manager);
    DEFINE_QPROPERTIES_CTORS(FSDIRECTBRIDGE_QPROPERTIES);
    setObjectName("FSDirectBridge");
}

QString FSDirectBridge::initialize(const QQDir & pluginDir)
{
    FUNCTION(pluginDir.path());
    OBJPOINTER(FrameSourceManager, _manager);

    if ( ! getLogFileName().isEmpty())
    {
        VariableSet logConfig("logConfig");
        logConfig.set("Device", "TxtFile");
        logConfig.set("FileName", getLogFileName());
        logConfig.set("Options", "TimeOfDay SeverityChar");
        logConfig.set("Filter/999/Report", "~");
        LOG(addDevice("IfSearch", logConfig));
    }

    ResultSet rset = _manager->initialize(pluginDir);
    RESULTS(rset);
    if (rset.is(Severity::Warning))
        return rset.toStringList().join(";");
    else
        return QString();
}

QString FSDirectBridge::start(void)
{
    FUNCTION();
    OBJPOINTER(FrameSourceManager, _manager);

    if (_interface)
    {
        _interface->shutdown();
        _interface = 0;
    }

    QUrl url(getUrl());
    if ( ! url.isValid())   return "URL not valid";

    FrameSourcePlugin * pi = _manager->plugin(url, this);
    if ( ! pi) return "No plugin for " + url.scheme();
    FSPPOINTER(FrameSourcePlugin, pi);
    _interface = new FrameSourceDirect(pi);
    OBJPOINTER(FrameSourceDirect, _interface);
    pi->setParent(_interface);

    VariableSet config;
    config.set("SampleMsec", getSampleMsec());
    _interface->autoStart(url,
                          config,
                          getChannel(),
                          getBegin(),
                          getEnd());
    CONNECT(_interface, SIGNAL(grabbed(int)),
            this, SLOT(takeGrab(int)));

    if (getMaxCache())
    {
        resume_timer = new QTimer(this);
        OBJPOINTER(QTimer, resume_timer);
        resume_timer->setInterval(500);
        CONNECT(resume_timer, SIGNAL(timeout()),
                this, SLOT(checkResume()));
    }

    return QString();
}

QString FSDirectBridge::stop(void)
{
    FUNCTION();
    if (_interface)
    {
        OBJPOINTER(FrameSourceDirect, _interface);
        _interface->shutdown();
        _interface = 0;
    }
    return QString();
}

QString FSDirectBridge::pause(void)
{
    FUNCTION();
    OBJPOINTER(FrameSourceDirect, _interface);

    if ( ! _interface->isPaused())
    {
        _interface->pause();
        setPaused(true);
    }
    return QString();
}

QString FSDirectBridge::resume(void)
{
    FUNCTION();
    OBJPOINTER(FrameSourceDirect, _interface);

    if (_interface->isPaused())
    {
        _interface->resume();
        setPaused(false);
    }
    return QString();
}

QStringList FSDirectBridge::schemes(void)
{
    return _manager->schemes();
}

void FSDirectBridge::takeGrab(int key)
{
    FUNCTION(key);
    OBJPOINTER(FrameSourceDirect, _interface);

    ImageEntity ie(_interface->take(key));
    if (ie.isNull())
    {
        ERROR("Null ImageEntity from Frame Source");
        return;
    }

    QByteArray ba(ie.value());
    QString format(ie.getImage_Format());
    QString frameId(ie.id());
    qint64 ems = ie.getImage_GrabTime();
    QString imageId = _cache->grab(ba, format, frameId, ems);
    if (imageId.isEmpty())
    {
        WARNING("Cache Grab Failed");
    }
    else
        EMIT(grabCached(imageId));

    if (getMaxCache()
            && _cache->grabSize() >= getMaxCache())
    {
        OBJPOINTER(QTimer, resume_timer);
        resume_timer->start();
        _interface->pause();
    }
}

void FSDirectBridge::checkResume(void)
{
    FUNCTION();

    if ( ! _interface->isPaused())
        resume_timer->stop();
    else if (_cache->grabSize() < getMaxCache())
    {
        if ( ! getPaused())
            _interface->resume();
        resume_timer->stop();
    }
}
