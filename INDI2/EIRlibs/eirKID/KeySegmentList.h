#ifndef KEYSEGMENTLIST_H
#define KEYSEGMENTLIST_H
#include "eirKID.h"

#include <QList>
#include "KeySegment.h"

class KeySegmentList : public QList<KeySegment>
{
public:
    KeySegmentList(void);
};

#endif // KEYSEGMENTLIST_H
