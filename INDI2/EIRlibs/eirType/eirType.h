/*! @file eirType.h Declarations for eirType class
*
*/
#ifndef EIRTYPE_H
#define EIRTYPE_H
#include "eirtype_global.h"
#ifndef __MODULE__
# define __MODULE__ "eirType"
#endif

#include <eirBase/Module.h>

typedef void * voidPtr;
typedef QList<int> intList;
typedef QVector<int> intVector;

class EIRTYPESHARED_EXPORT eirType : public Module
{

public:
    eirType(void);

public:
    static eirType * instance(void);

public:
    static eirType * instance_p;
};
Q_DECLARE_METATYPE(eirType);

int EIRTYPESHARED_EXPORT powerOfTwo(const int n);


#endif // EIRTYPE_H
