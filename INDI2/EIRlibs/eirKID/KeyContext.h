#ifndef KEYCONTEXT_H
#define KEYCONTEXT_H

#include <eirBase/EnumerationArray.h>

#include "KeyClass.h"
#include "KeySegment.h"

class KeyContext
{
public:
    KeyContext(void);

private:
    KeySegment db_seg;
    EnumerationArray<KeyClass, KeySegment> keyClass_seg_array;
    EnumerationArray<KeyClass, KeySegment> keySubclass_seg_array;
};

#endif // KEYCONTEXT_H
