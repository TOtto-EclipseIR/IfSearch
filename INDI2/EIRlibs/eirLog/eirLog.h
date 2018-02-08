#ifndef EIRLOG_H
#define EIRLOG_H
#include "eirlog_global.h"
#ifndef __MODULE__
# define __MODULE__ "eirType"
#endif

#include <eirBase/Module.h>

class EIRLOGSHARED_EXPORT eirLog : public Module
{

public:
    eirLog(void);

public:
    static eirLog * instance(void);

public:
    static eirLog * instance_p;
};
Q_DECLARE_METATYPE(eirLog);

#endif // EIRLOG_H
