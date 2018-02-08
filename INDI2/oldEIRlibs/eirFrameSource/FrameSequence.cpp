#include "FrameSequence.h"

FrameSequence::FrameSequence(INDImageClient *imClient,
                             QObject * parent)
    : QObject(parent)
    , _imClient(imClient)
{
}
