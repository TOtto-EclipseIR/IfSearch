#ifndef EIRCORE_H
#define EIRCORE_H
#ifndef __MODULE__
# define __MODULE__ "eirCore"
#endif
#include "eirCore_global.h"

#include <QtDebug>
#include <QtCore/QDir>

//#define INT64_MIN  (-9223372036854775807LL - 1)

/*! @class eirCore
  * @brief The eirCore library completes the core-level support for the EclipseIR libraries.
  */
class EIRCORESHARED_EXPORT eirCore
{
public:
    eirCore(void);
};

#endif // EIRCORE_H
