#include "ResultId.h"

ResultId::ResultId(const QString & inString)
    : BasicId(inString)
{
}

ResultId::ResultId(const char * chars)
    : BasicId(chars)
{
}

ResultId::ResultId(const char * chars,
           const char * chars1,
           const char * chars2,
           const char * chars3)
    : BasicId(chars, chars1, chars2, chars3)
{
}
