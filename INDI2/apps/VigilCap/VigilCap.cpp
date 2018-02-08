#include "VigilCap.h"
#include "version.h"
#include "../../common/version.h"

#include <QtCore/QTimer>

#include <eirTypes/MillisecondDelta.h>
#include <eirTypes/MillisecondTime.h>
#include <eirCore/VariableSettingsList.h>
#include <eirExe/Log.h>
#include <ImVigil/VigilDvr.h>
#include <ImVigil/VigilResult.h>
#include <ImVigil/VigilLiveFeed.h>
#include <ImVigil/VigilPlayback.h>

VigilCap::VigilCap(int argc, char ** argv)
    : Console(argc, argv)
    , vigil_dvr(0)
    , vigil_live(0)
    , vigil_play(0)
{
    setObjectName("VigilCap");
    QTimer::singleShot(10, this, SLOT(initialize()));
}

void VigilCap::initialize(void)
{
    FUNCTION();
    SETVERSION();
    initializeOptions();
    initializeLogger();

    options()->setDefault("URL", "vplay://user:123@fast.3xlogic.com/0");
    options()->setDefault("OutputDir", "./Output/@");
    options()->setDefault("Log/troll/Device", "troll");
    options()->setDefault("Log/troll/Options", "TimeOfDay LevelChar");
    options()->setDefault("Log/troll/Filter/999/Report", "~Trace");

    if (arg(0).isEmpty())
        vigil_url.setUrl(options()->value("URL").toString());
    else
        vigil_url.setUrl(arg(0));
    if ( ! vigil_url.isValid())
    {
        ERROR("Vigil URL is not valid: %1", vigil_url.toString());
        QTimer::singleShot(10, this, SLOT(quit()));
        return;
    }

    if ("vlive" == vigil_url.scheme() || "vplay" == vigil_url.scheme())
    {
        INFO("URL = {%1}", vigil_url.toString());
    }
    else
    {
        ERROR("Vigil URL is not Vigil: %1", vigil_url.toString());
        QTimer::singleShot(10, this, SLOT(quit()));
        return;
    }

    QString outDirName(options()->value("OutputDir").toString());
    if ( ! arg(1).isEmpty()) outDirName = arg(1);
    if ( ! output_dir.exists(outDirName))
        EXPECT(output_dir.mkpath(outDirName));
    EXPECT(output_dir.cd(outDirName));
    INFO("OutputDir = [%1]", output_dir.absolutePath())

    QTimer::singleShot(10, this, SLOT(login()));
}

void VigilCap::login(void)
{
    FUNCTION();
    vigil_dvr = new VigilDvr(this);
    NULPOINTER(vigil_dvr);    
    if ( ! vigil_dvr->isError())
        if (vigil_dvr->login(vigil_url))
        {
            PROGRESS("Logged into %1", vigil_url);
            INFO("Server Name: %1", vigil_dvr->serverName());
            QTimer::singleShot(10, this, SLOT(start()));
            return;
        }
    ERROR("VigilDvr error: ", vigil_dvr->errorString());
    QTimer::singleShot(10, this, SLOT(quit()));
}

void VigilCap::start(void)
{
    OBJPOINTER(VigilDvr, vigil_dvr);
    if ("vlive" == vigil_url.scheme())
    {
        vigil_live = new VigilLiveFeed(vigil_dvr);
        OBJPOINTER(VigilLiveFeed, vigil_live);
        if (vigil_live->isError()) return;
        vigil_live->start(vigil_url.path().toInt());
        if (vigil_live->isError())
        {
            ERROR("VigilLive error: ", vigil_live->errorString());
            QTimer::singleShot(10, this, SLOT(quit()));
        }
    }
    else if ("vplay" == vigil_url.scheme())
    {
        vigil_play = new VigilPlayback(vigil_dvr);
        OBJPOINTER(VigilPlayback, vigil_play);
        if (vigil_play->isError()) return;

        MillisecondTime current_mst(MillisecondTime::current());
        MillisecondTime start_mst;
        MillisecondTime end_mst;
        MillisecondDelta sample_msd;
        MillisecondDelta duration_msd;
        MillisecondDelta leadIn_msd;

        if (vigil_url.hasQueryItem("StartEms"))
            start_mst = vigil_url.queryItemValue("StartEms").toLongLong();
        if (vigil_url.hasQueryItem("EndEms"))
            end_mst = vigil_url.queryItemValue("EndEms").toLongLong();
        if (vigil_url.hasQueryItem("StartIso"))
            start_mst = MillisecondTime::fromIso(vigil_url.queryItemValue("StartIso"));
        if (vigil_url.hasQueryItem("EndIso"))
            end_mst = MillisecondTime::fromIso(vigil_url.queryItemValue("EndIso"));
        if (vigil_url.hasQueryItem("DurationMsec"))
            duration_msd = vigil_url.queryItemValue("DurationMsec").toLongLong();
        if (vigil_url.hasQueryItem("SampleMsec"))
            sample_msd = vigil_url.queryItemValue("SampleMsec").toLongLong();
        if (vigil_url.hasQueryItem("LeadInMsec"))
            leadIn_msd = vigil_url.queryItemValue("LeadInMsec").toLongLong();

        if (start_mst.isNull())         start_mst = current_mst;
        if (end_mst.isNull())           end_mst = current_mst;
        if ( ! leadIn_msd.isNull())     start_mst -= leadIn_msd;
        if ( ! duration_msd.isNull())   end_mst = current_mst + duration_msd;
        if (start_mst > current_mst)    start_mst = current_mst;
        if (end_mst > current_mst)      end_mst = current_mst;
        if (end_mst <= start_mst)       start_mst = end_mst - 300000;

        TRACE("Starting channel %1 from %2 to %3",
              vigil_url.path().mid(1),
              start_mst.toString(),
              end_mst.toString());
        vigil_play->setOutput(output_dir);
        CONNECT(vigil_play, SIGNAL(started()),
                this, SLOT(playStarted()));
        vigil_play->start(vigil_url.path().mid(1).toInt(),
                          start_mst,
                          end_mst,
                          sample_msd);
        if (vigil_play->isError())
        {
            ERROR("VigilPlay error: ", vigil_play->errorString());
            QTimer::singleShot(10, this, SLOT(quit()));
        }
    }
    else
    {
        ERROR("Vigil URL is not Vigil: %1", vigil_url.toString());
        QTimer::singleShot(10, this, SLOT(quit()));
        return;
    }
}

void VigilCap::liveStarted(void)
{
    FUNCTION();
    PROGRESS("liveStarted");
}

void VigilCap::playStarted(void)
{
    FUNCTION();
    PROGRESS("playStarted");
    DISCONNECT(vigil_play, SIGNAL(started()),
               this, SLOT(playStarted()));
    CONNECT(vigil_play, SIGNAL(grabbed()),
            this, SLOT(playNext()));
    CONNECT(vigil_play, SIGNAL(finished()),
            this, SLOT(playFinished()));
    QTimer::singleShot(10, this, SLOT(playNext()));
}

void VigilCap::playNext(void)
{
    //FUNCTION();
    QTimer::singleShot(10, vigil_play, SLOT(grab()));
}

void VigilCap::playFinished(void)
{
    FUNCTION();
    PROGRESS("Playback Finished");
    DISCONNECT(vigil_play, SIGNAL(grabbed()),
            this, SLOT(playNext()));
    QTimer::singleShot(10, this, SLOT(quit()));
}
