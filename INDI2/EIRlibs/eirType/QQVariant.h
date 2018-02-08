#ifndef QQVARIANT_H
#define QQVARIANT_H
#include "eirType.h"

#include <QList>
#include <QVariant>

class EIRTYPESHARED_EXPORT QQVariant : public QVariant
{
public:
    QQVariant(void);
};

typedef QList<QQVariant> QQVariantList;

#endif // QQVARIANT_H
