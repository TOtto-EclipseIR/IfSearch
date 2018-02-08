/*! @file Readable.h Declarations for Readable class
*
*/
#ifndef READABLE_H
#define READABLE_H
#include "eirType.h"

#include <QString>

class EIRTYPESHARED_EXPORT Readable : public QString
{
public:
    Readable(const QVariant & var);
    operator QString (void);

public:
    static QString formatted(const QString & format,
                             const QVariantList & vars);
};

#endif // READABLE_H
