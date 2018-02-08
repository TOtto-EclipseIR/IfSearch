#include "FramePerformance.h"

#include <QDateTime>

#include "FrameStatistics.h"

int FramePerformance::seq = 0;

FramePerformance::FramePerformance(const QString & section)
    : VariableGroup(section)
    , lastStartEms(0)
    , lastGrabEms(0)
    , lastCompleteEms(0)

{
    DEFINE_GROUPVARIABLES_CTORS(FRAMEPERFORMANCE_GROUPVARIABLES);
}

void FramePerformance::calculate(FrameStatistics * stats,
                                 const QString & imageId)
{
    int grabEms = stats->getGrab_ems();
    int startEms = stats->getStart_ems();
    int pixels = stats->getFrame_Size().width() * stats->getFrame_Size().height();
    int detectEms = stats->getDetect_ems();
    int detectMsec = stats->getDetect_msec();
    int completeEms = stats->getComplete_ems();

    setFrame_Seq(++seq);
    setImageId(imageId);
    setGrab_Latency_msec(grabEms ? (startEms - grabEms) : 0);
    setDetect_Overhead_msec(detectEms - startEms - detectMsec);
    setDetect_msec_perMP(detectMsec * 1024 * 1024 / pixels);
    setFace_Processing_msec(completeEms - detectEms);
    setTotal_Latency_msec(completeEms - (grabEms ? grabEms : startEms));
    setFrame_Overhead_msec(getTotal_Latency_msec() - detectMsec - getFace_Processing_msec());
    setGrab_Delta_msec(lastGrabEms ? (grabEms - lastGrabEms) : 0);
    setFrame_Delta_msec(lastStartEms ? (startEms - lastStartEms) : 0);
    setFrame_Idle_msec(lastCompleteEms ? (startEms - lastCompleteEms) : 0);

    setMatch_msec(stats->getMatch_msec());
    setMatch_count(stats->getMatch_count());
    if (getMatch_count())
        setMatch_msec_per_kcount(double(getMatch_msec()) / double(getMatch_count()) / 1000.0);

    lastGrabEms = grabEms, lastStartEms = startEms, lastCompleteEms = completeEms;
}
