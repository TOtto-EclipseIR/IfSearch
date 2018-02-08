#include "VigilCameraInfoList.h"

VigilCameraInfoList::VigilCameraInfoList()
{
}

bool VigilCameraInfoList::isValid(const int index) const
{
    return (index >= 0 && index < info_list.size());
}

QString VigilCameraInfoList::channelName(const int channelIndex)
{
    return isValid(channelIndex)
            ? info_list.at(channelIndex).name()
            : QString();
}
