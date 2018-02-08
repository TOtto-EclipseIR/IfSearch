#ifndef QQSizeF_H
#define QQSizeF_H
#include "eirTypes.h"

#include <QSizeF>

class EIRTYPESSHARED_EXPORT QQSizeF : public QSizeF
{
public:
    QQSizeF(const QSizeF & other=QSizeF());
    operator QSizeF(void) const;
    bool contains(const QSizeF size);
    bool contains(const qreal x, const qreal y);
    QSizeF united(const QSizeF size);
    QSizeF united(const qreal x, const qreal y);
    qreal area(void) const;
    qreal aspect(void) const;
    QSizeF powerOfTwo(void) const;
};

#endif // QQSizeF_H
