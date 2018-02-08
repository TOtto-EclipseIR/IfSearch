#include "KeyBuilder.h"

KeyBuilder::KeyBuilder(const KeyContext & context,
                       KeyRegistry * registry)
    : _context(context)
    , registry_p(registry)
    , partial_key(0)
{
}

KeyBuilder::KeyBuilder(const KeySegmentList & segments,
                       const KeyContext & context,
                       KeyRegistry * registry)
    : _context(context)
    , registry_p(registry)
    , segment_list(segments)
    , partial_key(0)
{

}

KeyBuilder::KeyBuilder(const UniversalKeyType value,
                       const KeySegmentList & segments,
                       const KeyContext & context,
                       KeyRegistry * registry)
    : _context(context)
    , registry_p(registry)
    , segment_list(segments)
    , partial_key(value)
{

}

void KeyBuilder::set(const UniversalKeyType value)
{

}

void KeyBuilder::append(const UniversalIdSegment & idSegment)
{

}

void KeyBuilder::generateNode(void)
{

}
