/*! \file Severity.h Declarations for Severity Enumeration
  *
  */

#ifndef SEVERITY_H
#define SEVERITY_H
#include "eirBase.h"

#include "Enumeration.h"

#define SEVERITY_ENUM(NV) \
    NV(Minimum,   ) /* 0*/       \
    NV(Alloc,     ) /* 1*/       \
    NV(Leave,     ) /* 2*/       \
    NV(EnterArg,  ) /* 3*/       \
    NV(Enter,     ) /* 4*/       \
    NV(Signal,    ) /* 5*/       \
    NV(Thread,    ) /* 6*/       \
    NV(Data,      ) /* 7*/       \
    NV(DumpHex,   ) /* 8*/       \
    NV(DumpVar,   ) /* 9*/       \
    NV(Dump,      ) /*10*/       \
    NV(Detail,    ) /*11*/       \
    NV(Trace,     ) /*12*/       \
    NV(Expect,    ) /*13*/       \
    NV(State,     ) /*14*/       \
    NV(Info,      ) /*15*/       \
    NV(LogMsg,    ) /*16*/       \
    NV(Progress,  ) /*17*/       \
    NV(ToDo,      ) /*18*/       \
    NV(Notice,    ) /*19*/       \
    NV(Watch,     ) /*20*/       \
    NV(Warning,   ) /*21*/       \
    NV(Stern,     ) /*22*/       \
    NV(Error,     ) /*23*/       \
    NV(Pointer,   ) /*24*/       \
    NV(Memory,    ) /*25*/       \
    NV(Assert,    ) /*26*/       \
    NV(Fatal,     ) /*27*/       \
    NV(Kill,      ) /*28*/       \
    NV(Shutdown,  ) /*29*/       \
    NV(Maximum,   ) /*30*/       \

class EIRBASESHARED_EXPORT Severity : public Enumeration
{
    DECLARE_ENUMERATION(Severity, SEVERITY_ENUM)
public:
    QChar toChar(void) const;
    bool isFatal(void) const;
    bool isError(void) const;
    bool isWarning(void) const;
};
Q_DECLARE_TYPEINFO(Severity, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Severity)

#endif // SEVERITY_H
