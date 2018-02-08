/*! @file DomVariant.h Declarations for DomVariant class
*
*/
#ifndef DOMVARIANT_H
#define DOMVARIANT_H
#include "eirType.h"

#include <QDomElement>

class EIRTYPESHARED_EXPORT DomVariant : public QDomElement
{
public:
    DomVariant(const QVariant & var);
    operator QDomElement (void);
};

#endif // DOMVARIANT_H
