/*! @file Parsable.h Declarations for Parsable class
*
*/
#ifndef PARSABLE_H
#define PARSABLE_H
#include "eirType.h"

#include <QString>

class EIRTYPESHARED_EXPORT Parsable : public QString
{
public:
    Parsable(const QVariant & var);
    operator QString (void);
};

#endif // PARSABLE_H
