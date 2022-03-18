/*! \file Severity.h Declarations for Severity Enumeration
  *
  */

#ifndef SEVERITY_H
#define SEVERITY_H
#include "eirBase.h"

#include "Enumeration.h"

#define SEVERITY_ENUM(NV) \
    NV(Minimum,   ) /* 0*/       \
    NV(Leave,     ) /* 1*/       \
    NV(EnterArg,  ) /* 2*/       \
    NV(Enter,     ) /* 3*/       \
    NV(Signal,    ) /* 4*/       \
    NV(Thread,    ) /* 5*/       \
    NV(Alloc,     ) /* 6*/       \
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

class Severity : public Enumeration
{
    DECLARE_ENUMERATION(Severity, SEVERITY_ENUM)
public:
    QChar toChar(void) const;
    bool isFatal(void) const;
    bool isError(void) const;
    bool isWarning(void) const;
};

#endif // SEVERITY_H
