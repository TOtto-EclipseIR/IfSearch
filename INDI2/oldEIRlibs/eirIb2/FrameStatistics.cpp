#include "FrameStatistics.h"

#include <QDateTime>

FrameStatistics::FrameStatistics(const QString & section)
    : VariableGroup(section)
{
    DEFINE_GROUPVARIABLES_CTORS(FRAMESTATISTICS_GROUPVARIABLES);
}

void FrameStatistics::start(const QDateTime grabTime,
                             const int cacheSize,
                             const QSize size)
{
    reset();
    setGrab_ems(grabTime.isValid()
               ? grabTime.toMSecsSinceEpoch()
               : 0);
    setCached(cacheSize);
    setStart_ems(QDateTime::currentMSecsSinceEpoch());
    setFrame_Size(size);
}

void FrameStatistics::detected(int totalFaces,
                                int rawObjects,
                                int strangeObjects,
                                int detectMsec)
{
    setTotal_Faces(totalFaces);
    setDetect_ems(QDateTime::currentMSecsSinceEpoch());
    setRaw_Objects(rawObjects);
    setStrange_Objects(strangeObjects);
    setDetect_msec(detectMsec);
}

void FrameStatistics::matched(int msec, int count)
{
    setMatch_msec(msec), setMatch_count(count);
}


void FrameStatistics::finish(const int fwPending,
                             const QString & cacheStatus)
{
    setWrites_Pending(fwPending);
    setCache_Status(cacheStatus);
    setComplete_ems(QDateTime::currentMSecsSinceEpoch());
}
