#include "FrameSourcePlugin.h"

#include <QtCore/QUrl>

#include "../eirBase/Severity.h"
#include "../eirExe/ResultId.h"
#include "../eirExe/Results.h"
#include "../eirExe/Log.h"

#include "FrameSourceBehavior.h"
#include "FrameSourceMachine.h"
#include "FrameSourceState.h"
#include "FrameSourceStatusIndex.h"

void FrameSourcePlugin::setUrl(const QUrl & url)
{
    FUNCTION(url.toString());
    _url = url;
}

void FrameSourcePlugin::connect(void)
{
    FUNCTION();
    EMIT(onConnect());
}

void FrameSourcePlugin::configure(const VariableSet & config)
{
    FUNCTION(config.name());
    config_vars = config;
    maxQueue_i = config_vars.value("MaxQueue", 4).toInt();
    EMIT(onConfigure());
}

void FrameSourcePlugin::start(const int channel,
                                  const QString & begin,
                                  const QString & end)
{
    FUNCTION(channel, begin, end);
    channel_i = channel;
    begin_mst = MillisecondTime::fromString(begin);
    end_mst = MillisecondTime::fromString(end);
    EMIT(onStart());
}

void FrameSourcePlugin::pause(void)
{
    FUNCTION();
    EMIT(onPause());
}

void FrameSourcePlugin::resume(void)
{
    FUNCTION();
    EMIT(onResume());
}

void FrameSourcePlugin::abort(void)
{
    FUNCTION();
    EMIT(onAbort());
}

void FrameSourcePlugin::stop(void)
{
    FUNCTION();
    EMIT(onStop());
}

void FrameSourcePlugin::shutdown(void)
{
    FUNCTION();
    EMIT(onShutdown());
}

bool FrameSourcePlugin::canGrab(void)
{
    FUNCTION();
    bool b = false;
    if (frame_image_list.size() < maxQueue_i
            && FrameSourceState::Ready == state())
        b = _behavior->canGrab();
    FNRETURN(b);
    return b;
}

void FrameSourcePlugin::grab(void)
{
    FUNCTION();
    NULPOINTER(_behavior);
    _behavior->preGrab();
    EMIT(doGrab());
}

bool FrameSourcePlugin::canTake(void) const
{
    FUNCTION();
    FNRETURN( ! frame_image_list.isEmpty());
    return ! frame_image_list.isEmpty();
}

ImageEntity FrameSourcePlugin::take(void)
{
    FUNCTION();
    return EXPECT(canTake())
            ? frame_image_list.takeFirst()
            : ImageEntity();
}
