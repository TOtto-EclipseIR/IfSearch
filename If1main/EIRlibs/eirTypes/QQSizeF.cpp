#include "QQSizeF.h"

#include "../eirBase/eirBase.h"


QQSizeF::QQSizeF(const QSizeF & other)
    : QSizeF(other)
{
}

QQSizeF::operator QSizeF(void) const
{
    return *this;
}

bool QQSizeF::contains(const QSizeF size)
{
    return size.width() <= QSizeF::width()
            && size.height() <= QSizeF::height();
}

bool QQSizeF::contains(const qreal x, const qreal y)
{
    return x < QSizeF::width() && y < QSizeF::height();
}

QSizeF QQSizeF::united(const QSizeF size)
{
    return QSizeF(qMax(QSizeF::width(),  size.width()),
                 qMax(QSizeF::height(), size.height()));
}

QSizeF QQSizeF::united(const qreal x, const qreal y)
{
    return QSizeF(qMax(QSizeF::width(),  x+1),
                 qMax(QSizeF::height(), y+1));
}

qreal QQSizeF::area(void) const
{
    return QSizeF::isValid()
            ? (QSizeF::width() * QSizeF::height())
            : 0;
}

qreal QQSizeF::aspect(void) const
{
    return (QSizeF::isValid() && QSizeF::width())
            ? ((qreal)QSizeF::height() / (qreal)QSizeF::width())
            : 0.0;

}

QSizeF QQSizeF::powerOfTwo(void) const
{
    return QSizeF(eirBase::powerOfTwo(QSizeF::width()),
                  eirBase::powerOfTwo(QSizeF::height()));
}
