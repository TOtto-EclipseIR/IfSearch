#ifndef KEYBUILDER_H
#define KEYBUILDER_H

#include "KeyContext.h"
#include "KeyRegistry.h"
#include "KeySegment.h"
#include "KeySegmentList.h"
class UniversalIdSegment;

class KeyBuilder
{
public:
    KeyBuilder(const KeyContext & context,
               KeyRegistry * registry);
    KeyBuilder(const KeySegmentList & segments,
               const KeyContext & context,
               KeyRegistry * registry);
    KeyBuilder(const UniversalKeyType value,
               const KeySegmentList & segments,
               const KeyContext & context,
               KeyRegistry * registry);
    void set(const UniversalKeyType value);
    void append(const UniversalIdSegment & idSegment);
    void generateNode(void);

private:
    const KeyContext _context;
    KeyRegistry * registry_p;
    KeySegmentList segment_list;
    UniversalKeyType partial_key;
};

#endif // KEYBUILDER_H
