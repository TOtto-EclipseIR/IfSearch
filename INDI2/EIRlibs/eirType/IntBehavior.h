/*! @file IntTypeBehavior.h Declarations for IntTypeBehavior class
*
*/
#ifndef TYPEBEHAVIORINT_H
#define TYPEBEHAVIORINT_H
#include "eirType.h"

#include "TypeBehavior.h"

class IntBehavior : public TypeBehavior
{
public:
    IntBehavior(void);
    virtual QString sortable(const QVariant & var);
    virtual QStringList hexdump(const QVariant & var,
                                const QString & title=QString());
    virtual QString parsable(const QVariant & var);
    virtual QVariant parsed(const int dtype, const QString & s);
    virtual QDomElement domVariant(const QVariant & var);
    virtual QVariant variantDom(const int dtype,
                                const QDomElement & de);
    static IntBehavior * instance(void);

private:
    static IntBehavior * instance_p;
};

#endif // TYPEBEHAVIORINT_H
