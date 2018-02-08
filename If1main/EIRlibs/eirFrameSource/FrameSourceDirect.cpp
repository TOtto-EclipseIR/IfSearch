#include "FrameSourceDirect.h"

#include <QtCore/QTimer>

#include "../eirCore/VariableSet.h"
#include "../eirExe/Log.h"

#include "FrameSourcePlugin.h"
#include "FrameSourceState.h"

FrameSourceDirect::FrameSourceDirect(FrameSourcePlugin * parent)
    : FrameSourceInterfaceBase(parent)
    , sample_timer(0)
{
    FUNCTION(OBJNAME(parent));
    setObjectName("FrameSourceDirect");
}

void FrameSourceDirect::configure(const VariableSet & config)
{
    FUNCTION(config.name());
    sample_mst = config.value("SampleMsec", 1000).toLongLong();
    FrameSourceInterfaceBase::configure(config);
}

void FrameSourceDirect::start(const int channel,
                              const QString & begin,
                              const QString & end)
{
    FUNCTION(channel);
    CONNECT(_plugin, SIGNAL(started(ResultSet)),
            this, SLOT(pluginStarted(ResultSet)));
    CONNECT(_plugin, SIGNAL(startError(ResultSet)),
            this, SLOT(pluginStarted(ResultSet)));
    CONNECT(_plugin, SIGNAL(complete(ResultSet)),
            this, SLOT(pluginComplete(ResultSet)));
    CONNECT(_plugin, SIGNAL(grabbed(ResultSet)),
            this, SLOT(pluginGrabbed(ResultSet)));
    CONNECT(_plugin, SIGNAL(grabTimeout(ResultSet)),
            this, SLOT(pluginGrabbed(ResultSet)));
    CONNECT(_plugin, SIGNAL(grabError(ResultSet)),
            this, SLOT(pluginGrabbed(ResultSet)));
    FrameSourceInterfaceBase::start(channel, begin, end);
}

void FrameSourceDirect::pluginStarted(ResultSet rset)
{
    FUNCTION(rset.toStringList());

    FrameSourceInterfaceBase::pluginStarted(rset);
    if (rset.is(Severity::Error)) return;

    if (sample_mst)
    {
        sample_timer = new QTimer(this);
        OBJPOINTER(QTimer, sample_timer);
        sample_timer->setInterval(sample_mst);
        CONNECT(sample_timer, SIGNAL(timeout()),
                this, SLOT(sample()));
        sample_timer->start();
    }
    else
        SHOOT(100, this, SLOT(sample()));
}

void FrameSourceDirect::pluginGrabbed(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    FrameSourceInterfaceBase::pluginGrabbed(rset);
    if ( ! sample_timer)
        SHOOT(100, this, SLOT(sample()));
}

void FrameSourceDirect::pluginComplete(ResultSet rset)
{
    FUNCTION(rset.toStringList());
    if (sample_timer)
    {
        delete sample_timer;
        sample_timer = 0;
    }
    EMIT(complete(rset));
}

void FrameSourceDirect::sample(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);

    if (_plugin->canTake())
    {
        int frameKey = FrameSourceInterfaceBase::enqueue(_plugin->take());
        frameKey_list.append(frameKey);
        EMIT(grabbed(frameKey));
    }

    TRACE("%1 state=%2", OBJNAME(_plugin), _plugin->stateName());
    if (FrameSourceState::Ready == _plugin->state())
    {
        if (_plugin->canGrab())
            _plugin->grab();
        else if ( ! sample_timer)
            SHOOT(100, this, SLOT(sample()));
    }

    if (_plugin->state() < FrameSourceState::Starting
            || _plugin->state() >= FrameSourceState::Finished)
        if (sample_timer)
            sample_timer->stop();
}

QList<int> FrameSourceDirect::frameKeys(const int n) const
{
    if (0 == n || n <= frameKey_list.size())
        return frameKey_list;
    else
    {
        QList<int> result(frameKey_list);
        while (result.size() > n)
            result.removeLast();
        return result;
    }
}

int FrameSourceDirect::takeKey(void)
{
    return frameKey_list.takeFirst();
}

QList<int> FrameSourceDirect::takeKeys(const int n)
{
    QList<int> result;

    if (0 == n || n <= frameKey_list.size())
    {
        result = frameKey_list;
        frameKey_list.clear();
    }
    else
    {
        while (result.size() < n)
            result.append(frameKey_list.takeFirst());
    }

    return result;
}
