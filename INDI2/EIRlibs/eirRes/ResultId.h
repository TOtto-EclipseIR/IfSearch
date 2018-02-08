#ifndef RESULTID_H
#define RESULTID_H
#include "eirRes.h"

#include <eirKID/BasicId.h>

class EIRRESSHARED_EXPORT ResultId : public BasicId
{
public:
    ResultId(const QString & inString=QString());
    ResultId(const char * chars);
    ResultId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);

};
Q_DECLARE_TYPEINFO(ResultId, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(ResultId)

#endif // RESULTID_H
