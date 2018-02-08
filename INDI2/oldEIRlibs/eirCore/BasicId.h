#ifndef BasicID_H
#define BasicID_H
#include "eirCore.h"

#include "../eirBase/AbstractIdString.h"

class EIRCORESHARED_EXPORT BasicId : public AbstractIdString
{
public:
    BasicId(const QString & inString=QString());
    BasicId(const char * chars);
    BasicId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);
};

#endif // BasicID_H
