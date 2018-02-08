#ifndef BasicID_H
#define BasicID_H
#include "eirKID.h"

#include "BaseIdString.h"

class EIRKIDSHARED_EXPORT BasicId : public BaseIdString
{
public:
    BasicId(const QString & inString=QString());
    BasicId(const char * chars);
    BasicId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);
    static int metaType(void);
    static const char * metaTypeName(void);
};
Q_DECLARE_TYPEINFO(BasicId, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(BasicId)

#endif // BasicID_H
