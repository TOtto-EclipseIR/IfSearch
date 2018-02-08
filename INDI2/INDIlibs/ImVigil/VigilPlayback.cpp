#include "VigilPlayback.h"

#include <QtGui/QWidget>

#include "../../EIRlibs/eirExe/Log.h"
#include "VigilDvr.h"

VigilPlayback::VigilPlayback(VigilDvr *parent)
    : QObject(parent)
    , _dvr(parent)
    , _play(0)
    , _widget(0)
{
    FUNCTION();
    setObjectName("VigilPlayback");
    if (( ! _dvr) || _dvr->isError())
    {
        error_s = "DVR is in error";
        return;
    }

    _play = _dvr->createPlayback();
    if ( ! _play)
    {
        error_s = "Failed Playback object";
        return;
    }
}

void VigilPlayback::setOutput(const QDir & dir,
                              const QString & frameIdFormat,
                              const int quality)
{
    FUNCTION(dir.absolutePath(), frameIdFormat, quality);
    frame_idGenerator.setFrameFormat(frameIdFormat);
    output_dir = dir;
    quality_i = quality;
}

bool VigilPlayback::isError(void) const
{
    return ( ! error_s.isEmpty())
            || ( ! _dvr) || _dvr->isError()
            || ( ! _play);
}

QString VigilPlayback::errorString(void) const
{
    return error_s;
}

void VigilPlayback::start(const int channelIndex,
                          const MillisecondTime start,
                          const MillisecondTime end,
                          const MillisecondDelta sample)
{
    FUNCTION(channelIndex, (qint64)start,
             (qint64)end, (qint64)sample);
    if (isError())  return;

    if (_play->FetchFootage(channelIndex,
                            start.toDateTime(),
                            end.toDateTime()))
    {
        TRACE("Playback Fetched");
    }
    else
    {
        error_s = "Playback Fetch failed";
        ERROR(error_s);
        return;
    }

    if (_play->View(0))
    {
        TRACE("Playback View Started");
        start_mst = start;
        end_mst = end;
        sample_msd = (sample > 0) ? sample : MillisecondDelta(1000);
        frame_n = 0;
        emit started();
    }
    else
    {
        error_s = "Playback View failed";
        ERROR(error_s);
        return;
    }
}

void VigilPlayback::grab(void)
{
/*
    if (cue_mst.isNull())
        cue_mst = start_mst;
    else
        cue_mst += sample_msd;
*/
    MillisecondTime cue_mst(start_mst + frame_n * sample_msd);
    //TRACE("Cue: %1 [%2]", cue_mst.toString(), qint64(cue_mst));

    if (cue_mst > end_mst)
    {
        TRACE("Playback Finished");
        emit finished();
        return;
    }
    _play->CueToTimestamp(cue_mst.toDateTime());

    frame_idGenerator.setFrameNumber(++frame_n);
    frame_idGenerator.setModifiedMst(cue_mst);
    BasicId frameId = frame_idGenerator.frame("Grab");
    QString fileName = output_dir.absoluteFilePath(frameId + ".JPG");
    fileName = QDir::toNativeSeparators(fileName);
    TRACE("Export %2 %1", fileName, frame_n);
    EXPECT(_play->ExportJPG(fileName, quality_i, false));
    emit grabbed();
}
