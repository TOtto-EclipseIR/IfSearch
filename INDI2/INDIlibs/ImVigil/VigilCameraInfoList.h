#ifndef VIGILCAMERAINFOLIST_H
#define VIGILCAMERAINFOLIST_H
#include "ImVigil.h"

#include "VigilCameraInfo.h"

class IMVIGILSHARED_EXPORT VigilCameraInfoList
{
public:
    VigilCameraInfoList();
    bool isValid(const int index) const;
    QString channelName(const int channelIndex);

private:
    QList<VigilCameraInfo> info_list;
};

#endif // VIGILCAMERAINFOLIST_H
