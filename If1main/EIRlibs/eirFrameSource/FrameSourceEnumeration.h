#ifndef FRAMESOURCEENUMERATION_H
#define FRAMESOURCEENUMERATION_H
#include "eirFrameSource.h"

#include <QtCore/QUrl>

#include "FrameSourceChannelInfo.h"

class EIRFRAMESOURCESHARED_EXPORT FrameSourceEnumeration
{
public:
    FrameSourceEnumeration(void);

private:
    QUrl _url;
    int channels_n;
    QList<FrameSourceChannelInfo> channel_list;
};

#endif // FRAMESOURCEENUMERATION_H
