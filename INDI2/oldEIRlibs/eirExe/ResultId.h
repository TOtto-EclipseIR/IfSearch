#ifndef RESULTID_H
#define RESULTID_H
#include "eirExe.h"

#include "../eirCore/BasicId.h"

class EIREXESHARED_EXPORT ResultId : public BasicId
{
public:
    ResultId(const QString & inString=QString());
    ResultId(const char * chars);
    ResultId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);

};

#endif // RESULTID_H
