#ifndef STRINGBEHAVIOR_H
#define STRINGBEHAVIOR_H
#include "eirType.h"

#include "TypeBehavior.h"

class EIRTYPESHARED_EXPORT StringBehavior : public TypeBehavior
{
public:
    StringBehavior(void);
    StringBehavior(const intList metaTypes);
    virtual QString sortable(const QVariant & var);
    virtual QStringList hexdump(const QVariant & var,
                                const QString & title=QString());
    virtual QString parsable(const QVariant & var);
    virtual QVariant parsed(const QString & s);
    virtual QDomElement domVariant(const QVariant & var);
    virtual QVariant variantDom(const QDomElement & de);
    static StringBehavior * instance(void);

private:
    static StringBehavior * instance_p;
};

#endif // STRINGBEHAVIOR_H
