#ifndef FRAMEPERFORMANCE_H
#define FRAMEPERFORMANCE_H

#include "../eirVariable/VariableGroup.h"
class FrameStatistics;

#define FRAMEPERFORMANCE_GROUPVARIABLES(ITD) \
    ITD(Frame_Seq, int, 0) \
    ITD(ImageId, QString, "") \
    ITD(Grab_Latency_msec, int, 0) \
    ITD(Detect_Overhead_msec, int, 0) \
    ITD(Detect_msec_perMP, int, 0) \
    ITD(Face_Processing_msec, int, 0) \
    ITD(Frame_Overhead_msec, int, 0) \
    ITD(Total_Latency_msec, int, 0) \
    ITD(Grab_Delta_msec, int, 0) \
    ITD(Frame_Delta_msec, int, 0) \
    ITD(Frame_Idle_msec, int, 0) \


class FramePerformance : public VariableGroup
{
public:
    DECLARE_GROUPVARIABLES(FRAMEPERFORMANCE_GROUPVARIABLES);
    FramePerformance(const QString & section=QString());
    void calculate(FrameStatistics * stats,
                   const QString & imageId);

private:
    static int seq;
    int lastStartEms;
    int lastGrabEms;
    int lastCompleteEms;
};

#endif // FRAMEPERFORMANCE_H
