#include "VigilLiveFeed.h"

#include "../../EIRlibs/eirExe/Log.h"
#include "VigilDvr.h"

VigilLiveFeed::VigilLiveFeed(VigilDvr * parent)
    : QObject(parent)
    , _dvr(parent)
    , _live(0)
    , _widget(0)
{
    FUNCTION();
    setObjectName("VigilLiveFeed");

    if (( ! _dvr) || _dvr->isError())
    {
        error_s = "DVR is in error";
        return;
    }

    _live = _dvr->createLiveFeed();
    if ( ! _live)
    {
        error_s = "Failed Playback object";
        return;
    }
}

bool VigilLiveFeed::isError(void) const
{
    return ( ! error_s.isEmpty()) || ( ! _dvr) || _dvr->isError();
}

QString VigilLiveFeed::errorString(void) const
{
    return error_s;
}


void VigilLiveFeed::start(int channelIndex)
{
    FUNCTION(channelIndex);
    if (isError())  return;

    if (_live->View(channelIndex, 0))
    {
        PROGRESS("Channel %1 started", channelIndex);
        emit started();
    }
    else
    {
        ERROR("Unable to start channel "
              + QString::number(channelIndex));
        error_s = "Unable to start channel "
                + QString::number(channelIndex);
    }
}
