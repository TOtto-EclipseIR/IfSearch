#include "FrameSourceInterfaceBase.h"

#include "../eirCore/VariableSet.h"
#include "../eirImage/ImageEntity.h"
#include "../eirImage/MessageImageConfiguration.h"

#include "FrameSourcePlugin.h"

FrameSourceInterfaceBase::FrameSourceInterfaceBase(FrameSourcePlugin * parent)
    : QObject(parent)
    , _plugin(parent)
    , miEnable_b(false)
    , autoStart_b(false)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    setObjectName("FrameSourceInterfaceBase");
    CONNECT(_plugin, SIGNAL(initialized(ResultSet)),
            this, SLOT(pluginInitialized(ResultSet)));
    CONNECT(_plugin, SIGNAL(connected(ResultSet)),
            this, SLOT(pluginConnected(ResultSet)));
    CONNECT(_plugin, SIGNAL(connectError(ResultSet)),
            this, SLOT(pluginConnected(ResultSet)));
    CONNECT(_plugin, SIGNAL(configured(ResultSet)),
            this, SLOT(pluginConfigured(ResultSet)));
    CONNECT(_plugin, SIGNAL(complete(ResultSet)),
            this, SLOT(pluginComplete(ResultSet)));
}

void FrameSourceInterfaceBase::autoStart(
       const QUrl & url,
       const VariableSet & config,
       const int channel,
       const QString & begin,
       const QString & end)
{
    FUNCTION(url.toString(), config.name(),
             channel, begin+"~"+end);
    auto_url = url;
    autoConfig_vars = config;
    autoChannel_i = channel;
    autoBegin_s = begin;
    autoEnd_s = end;
    autoStart_b = true;
    setUrl(auto_url);
}

void FrameSourceInterfaceBase::pluginInitialized(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    FSPPOINTER(FrameSourcePlugin, _plugin);
    EMIT(initialized(rset));

    MessageImageConfiguration miConfig
           ; // = _plugin->configuration().exportSection("MessageImage");
    miEnable_b = true; //miConfig.value("Enable").toBool();
    if (miEnable_b)
    {
        miConfig.initialize();
        DUMPVSET(miConfig);
        _miGenerator.setConfig(miConfig);
    }

    if (rset.is(Severity::Error))
    {
        Result errorResult = rset.findSeverity(Severity::Error);
        if ( ! errorResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(errorResult);
            EMIT(messageImage(ie));
        }
        autoStart_b = false;
    }
    else
    {
        Result progressResult = rset.findSeverity(Severity::Progress);
        if ( ! progressResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(progressResult);
            EMIT(messageImage(ie));
        }
        if (autoStart_b)
        {
            TRACE("FrameSourceInterfaceBase::connect(%1)",
                  auto_url.toString());
            SHOOT(100, this, SLOT(doConnect()));
        }
    }
}

void FrameSourceInterfaceBase::doConnect(void)
{
    FUNCTION();
    connect();
}

void FrameSourceInterfaceBase::pluginConnected(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    EMIT(connected(rset));

    if (rset.is(Severity::Error))
    {
        Result errorResult = rset.findSeverity(Severity::Error);
        if ( ! errorResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(errorResult);
            EMIT(messageImage(ie));
        }
        autoStart_b = false;
    }
    else
    {
        Result progressResult = rset.findSeverity(Severity::Progress);
        if ( ! progressResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(progressResult);
            EMIT(messageImage(ie));
        }
        if (autoStart_b)
            SHOOT(100, this, SLOT(doConfigure()));
    }
}

void FrameSourceInterfaceBase::doConfigure(void)
{
    FUNCTION();
    configure(autoConfig_vars);
}

void FrameSourceInterfaceBase::pluginConfigured(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    EMIT(configured(rset));

    if (rset.is(Severity::Error))
    {
        Result errorResult = rset.findSeverity(Severity::Error);
        if ( ! errorResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(errorResult);
            EMIT(messageImage(ie));
        }
        autoStart_b = false;
    }
    else
    {
        Result progressResult = rset.findSeverity(Severity::Progress);
        if ( ! progressResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(progressResult);
            EMIT(messageImage(ie));
        }
        if (autoStart_b)
            SHOOT(100, this, SLOT(doStart()));
        autoStart_b = false;
    }
}

void FrameSourceInterfaceBase::doStart(void)
{
    FUNCTION();
    TRACE(OBJNAME(this));
    start(autoChannel_i, autoBegin_s, autoEnd_s);
}

void FrameSourceInterfaceBase::pluginStarted(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    EMIT(started(rset));

    if (rset.is(Severity::Error))
    {
        Result errorResult = rset.findSeverity(Severity::Error);
        if ( ! errorResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(errorResult);
            EMIT(messageImage(ie));
        }
    }
    else
    {
        Result progressResult = rset.findSeverity(Severity::Progress);
        if ( ! progressResult.isNull() && miEnable_b)
        {
            ImageEntity ie = _miGenerator.generate(progressResult);
            EMIT(messageImage(ie));
        }
    }
}

void FrameSourceInterfaceBase::pluginGrabbed(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    if (rset.is(Severity::Warning))
    {
        foreach (Result res, rset.list())
        {
            if (res.severity().isWarning())
            {
                EMIT(warning(res));
                if (miEnable_b)
                {
                    ImageEntity ie = _miGenerator.generate(res);
                    EMIT(messageImage(ie));
                }
            }
        }
    }
    EMIT(grabbed(rset));
}

void FrameSourceInterfaceBase::pluginStopped(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    EMIT(stopped(rset));
}

void FrameSourceInterfaceBase::pluginComplete(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    if (rset.is(Severity::Progress))
    {
        foreach (Result res, rset.list())
        {
            if (res.severity() >= Severity::Progress)
            {
                if (miEnable_b)
                {
                    ImageEntity ie = _miGenerator.generate(res);
                    EMIT(messageImage(ie));
                }
                if (res.severity().isWarning())
                {
                    EMIT(warning(res));
                }
            }
        }
    }
    EMIT(grabbed(rset));
}

qint64 FrameSourceInterfaceBase::enqueue(ImageEntity frameEntity)
{
    FUNCTION(frameEntity.id());
    qint64 key = (int)MillisecondTime::current();
    frameEntity.setKey(key);
    key_entity_map.insert(key, frameEntity);
    FNRETURN(key);
    return key;
}

ImageEntity FrameSourceInterfaceBase::take(int frameKey)
{
    FUNCTION(frameKey);
    EXPECT(key_entity_map.contains(frameKey));
    DUMPVAR(key_entity_map.size());
    FNRETURN(key_entity_map.value(frameKey).value().size());
    return key_entity_map.take(frameKey);
}

QStringList FrameSourceInterfaceBase::schemeList(void) { FUNCTION(); return _plugin->schemeList(); }
bool FrameSourceInterfaceBase::supports(const QUrl &url) { FUNCTION(url.toString()); return _plugin->supports(url); }
bool FrameSourceInterfaceBase::supportsScheme(const QString & scheme) {  FUNCTION(scheme); return _plugin->supportsScheme(scheme); }

void FrameSourceInterfaceBase::setUrl(const QUrl & url) { FUNCTION(url.toString()); _plugin->setUrl(url); }
void FrameSourceInterfaceBase::setResultSeverity(const Severity sev) { FUNCTION(); _plugin->setResultSeverity(sev); }
void FrameSourceInterfaceBase::connect(void) { FUNCTION(); _plugin->connect(); }
void FrameSourceInterfaceBase::configure(const VariableSet & config) { FUNCTION(); _plugin->configure(config);  }
void FrameSourceInterfaceBase::start(const int channel, const QString & begin, const QString & end) { FUNCTION(channel, begin, end); _plugin->start(channel, begin, end); }
void FrameSourceInterfaceBase::pause(void) { FUNCTION(); _plugin->pause(); }
void FrameSourceInterfaceBase::resume(void) { FUNCTION();  _plugin->resume(); }
void FrameSourceInterfaceBase::abort(void) { FUNCTION(); _plugin->abort(); }
void FrameSourceInterfaceBase::stop(void) { FUNCTION(); _plugin->stop(); }

void FrameSourceInterfaceBase::shutdown(void)
{
    FUNCTION();
    _plugin->shutdown();
    _plugin->deleteLater();
    _plugin = 0;
}

bool FrameSourceInterfaceBase::canGrab(void) { FUNCTION(); return _plugin->canGrab(); }
void FrameSourceInterfaceBase::grab(void) { FUNCTION(); _plugin->grab(); }
bool FrameSourceInterfaceBase::canTake(void) const { FUNCTION(); return _plugin->canTake(); }
ImageEntity FrameSourceInterfaceBase::take(void) { FUNCTION(); return _plugin->take(); }

VariableSet FrameSourceInterfaceBase::configuration(void) const { FUNCTION(); return _plugin->configuration(); }
ResultSet FrameSourceInterfaceBase::status(void) const { FUNCTION(); return _plugin->status(); }
ResultSet FrameSourceInterfaceBase::results(void) const { FUNCTION(); return _plugin->results(); }
ResultSet FrameSourceInterfaceBase::takeResults(void) { FUNCTION(); return _plugin->takeResults(); }

bool FrameSourceInterfaceBase::isInitialized(void) const { FUNCTION(); return _plugin->isInitialized(); }
bool FrameSourceInterfaceBase::isConfigured(void) const { FUNCTION(); return _plugin->isConfigured(); }
bool FrameSourceInterfaceBase::isStarted(void) const { FUNCTION(); return _plugin->isStarted(); }
bool FrameSourceInterfaceBase::isPaused(void) const { FUNCTION(); return _plugin->isPaused(); }
bool FrameSourceInterfaceBase::isFinished(void) const { FUNCTION(); return _plugin->isFinished(); }

QDomElement FrameSourceInterfaceBase::enumerate(const int channel) { FUNCTION(channel); return _plugin->enumerate(); }
QString FrameSourceInterfaceBase::channelId(const int channel) { FUNCTION(channel); return _plugin->channelId(); }

