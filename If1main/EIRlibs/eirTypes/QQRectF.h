#ifndef QQRECTF_H
#define QQRECTF_H
#include "eirTypes.h"

#include <QtCore/QRectF>

class EIRTYPESSHARED_EXPORT QQRectF
        : public QRectF
{
public:
    QQRectF(const QRectF rf=QRectF());
    QQRectF(const QSizeF size,
            const QPointF center);

    bool isUnit(void) const;
    QRect rect(const int iWidth=0);
    QSize size(const int iWidth=0);
    QString toString(void) const;
};

#endif // QQRECTF_H
