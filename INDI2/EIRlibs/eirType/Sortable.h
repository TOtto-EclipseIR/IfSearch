/*! @file Readable.h Declarations for Readable class
*
*/
#ifndef SORTABLE_H
#define SORTABLE_H
#include "eirType.h"

#include <QString>

class EIRTYPESHARED_EXPORT Sortable : public QString
{
public:
    Sortable(const QVariant & var);
    operator QString (void);
};

#endif // SORTABLE_H
