/*! @file VariantDom.h Declarations for VariantDom class
*
*/
#ifndef VARIANTDOM_H
#define VARIANTDOM_H
#include "eirType.h"

#include <QVariant>

class QDomElement;

class EIRTYPESHARED_EXPORT VariantDom : public QVariant
{
public:
    VariantDom(const QDomElement & de);
    operator QVariant (void);
};

#endif // VARIANTDOM_H
