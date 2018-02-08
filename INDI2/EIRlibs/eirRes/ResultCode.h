#ifndef RESULTKEY_H
#define RESULTKEY_H
#include "eirRes.h"

#include <eirType/EightCC.h>

#define RESULTKEY_ENUM(NV) \

class EIRRESSHARED_EXPORT ResultCode : public EightCC
{
public:
    ResultCode(void);
    ResultCode(const quint64 value);
    ResultCode(const char * pc);
};
Q_DECLARE_TYPEINFO(ResultCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_METATYPE(ResultCode)

#endif // RESULTKEY_H
