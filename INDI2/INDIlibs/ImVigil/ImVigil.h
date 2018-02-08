#ifndef IMVIGIL_H
#define IMVIGIL_H
#ifndef __MODULE__
# define __MODULE__ "ImVigil"
#endif
#include "ImVigil_global.h"

#include <QtCore/QSet>

#include "VigilSDK.h"



class IMVIGILSHARED_EXPORT ImVigil
{
public:
    ImVigil(void);
    ~ImVigil();

private:
    static bool coInitialized_b;
    static QSet<ImVigil *> instance_set;
};

#endif // IMVIGIL_H
