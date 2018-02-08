#ifndef RESULTKEY_H
#define RESULTKEY_H
#include "eirExe.h"

#include "../eirBase/EightCC.h"

#define RESULTKEY_ENUM(NV) \

class EIREXESHARED_EXPORT ResultCode : public EightCC
{
public:
    ResultCode(void);
    ResultCode(const quint64 value);
    ResultCode(const char * pc);
};

#endif // RESULTKEY_H
