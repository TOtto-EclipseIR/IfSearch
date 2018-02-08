#ifndef CLASSID_H
#define CLASSID_H
#include "eirKID.h"

#include "BaseIdString.h"

class EIRKIDSHARED_EXPORT ClassId : public BaseIdString
{
public:
    ClassId(const QString & inString=QString());
    ClassId(const char * chars);
    ClassId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);
    static int metaType(void);
    static const char * metaTypeName(void);
};
Q_DECLARE_TYPEINFO(ClassId, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(ClassId)

#endif // CLASSID_H
