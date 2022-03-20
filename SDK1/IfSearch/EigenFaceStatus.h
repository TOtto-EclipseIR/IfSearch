#ifndef EIGENFACESTATUS_H
#define EIGENFACESTATUS_H
#include <qglobal.h>

#include "../../INDI2/oldEIRlibs/eirBase/Enumeration.h"

#define EIGENFACESTATUS_ENUM(NV) \
    NV(New, =0)     \
    NV(First,)      \
    NV(Active,)     \
    NV(Primary,)    \
    NV(Composite,)  \
    NV(Manual,)     \
    NV(Adjusted,)   \
    NV(Seed,)       \
    NV(Expired,)    \
    NV(Inactive,)   \
    NV(Error,)      \
    NV(Deleted,)    \
    NV(Merged,)     \
    NV(User0,)      \
    NV(User1,)      \
    NV(User2,)      \
    NV(User3,)      \
    NV(User4,)      \
    NV(User5,)      \
    NV(User6,)      \
    NV(User7,)      \
    NV(User8,)      \
    NV(User9,)      \

class EigenFaceStatus : public Enumeration
{
public:
    DECLARE_ENUMERATION(EigenFaceStatus, EIGENFACESTATUS_ENUM)
};

#endif // EIGENFACESTATUS_H
