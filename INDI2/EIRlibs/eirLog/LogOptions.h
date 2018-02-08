/*! @file LogOptions.h  Definition of LogLevel enumeration
*
*/
#ifndef LOGOPTIONS_H
#define LOGOPTIONS_H
#include "eirLog.h"

class QUrlQuery;

#include <eirBase/Enumeration.h>

#define LOGOPTIONS_ENUM(NV) \
    NV(Null,            = 0)  \
    NV(TimeOfDay,       = 0x00000001) /* 0*/       \
    NV(TimeDelta,       = 0x00000002) /* 1*/       \
    NV(NoTime,          = 0x00000004) /* 2*/       \
    NV(Time3,           = 0x00000008) /* 3*/       \
    NV(ShowFile,        = 0x00000010) /* 4*/       \
    NV(ShowLine,        = 0x00000020) /* 5*/       \
    NV(ShowClass,       = 0x00000040) /* 6*/       \
    NV(ShowFunction,    = 0x00000080) /* 7*/       \
    NV(ShowThread,      = 0x00000100) /* 8*/       \
    NV(Show9,           = 0x00000200) /* 9*/       \
    NV(Show10,          = 0x00000400) /*10*/       \
    NV(Show11,          = 0x00000800) /*11*/       \
    NV(LevelChar,       = 0x00001000) /*12*/       \
    NV(LevelName,       = 0x00002000) /*13*/       \
    NV(NoLevel,         = 0x00004000) /*14*/       \
    NV(Level15,         = 0x00008000) /*15*/       \
    NV(Opt16,           = 0x00010000) /*16*/       \
    NV(Opt17,           = 0x00020000) /*17*/       \
    NV(Opt18,           = 0x00040000) /*18*/       \
    NV(Opt19,           = 0x00080000) /*19*/       \
    NV(Opt20,           = 0x00100000) /*20*/       \
    NV(Opt21,           = 0x00200000) /*21*/       \
    NV(Opt22,           = 0x00400000) /*22*/       \
    NV(Opt23,           = 0x00800000) /*23*/       \
    NV(Out24,           = 0x01000000) /*24*/       \
    NV(Out25,           = 0x02000000) /*25*/       \
    NV(TraceOpts,       = 0x00002131) /* LevelName | ShowThread,File,Line | TimeOfDay */ \
    NV(UserOpts,        = 0x00001001) /* LevelChar | TimeOfDay */ \

class EIRLOGSHARED_EXPORT LogOptions : public Enumeration
{
    DECLARE_ENUMERATION(LogOptions, LOGOPTIONS_ENUM)
public:
    static int maskFrom(const QUrlQuery & query);
};
Q_DECLARE_TYPEINFO(LogOptions, Q_MOVABLE_TYPE);

#endif // LOGOPTIONS_H
