#include "TransformProperties.h"
#include <QtGui/QTransform>

#include "InfoMacros.h"

TransformProperties::TransformProperties(QObject *parent) :
    QObject(parent)
{
    setObjectName("TransformProperties");

}

bool TransformProperties::isNull(void) const
{
    if ( ! qIsNull(getRotate()))
        return false;
    if (getScale() > 0.0 && ! qIsNull(getScale() - 1.0))
        return false;
    if (getAspect() > 0.0 && ! qIsNull(getAspect() - 1.0))
        return false;
    return true;
}

QTransform TransformProperties::transform(void) const
{
    QTransform xf;
    if (isNull() && getCrop().isNull())
        return xf;

    qreal deg = - getRotate();
    qreal sca = getScale();
    qreal asp = getAspect();
    qreal sy = (sca > 0.0) ? sca : 1.0;
    qreal sx = (asp > 0.0) ? (sy * asp) : sy;
    if ( ! qIsNull(deg))
        xf.rotate(deg);
    if ( ! qIsNull(sx - 1.0) || ! qIsNull(sy - 1.0))
    {
        TRACE("scale(%1,%2)", sx, sy);
        xf.scale(sx, sy);
    }

    // TODO: if ( ! getCrop().isNull())

    return xf;
}

int TransformProperties::dimension(const QSizeF sz) const
{
    return qRound(qMax(sz.width()  * getScale(),
                       sz.height() * getScale()));
}

qreal TransformProperties::getScale() const
{
    return mScale;
}

void TransformProperties::setScale(qreal newScale)
{
    if (qFuzzyCompare(mScale, newScale))
        return;
    mScale = newScale;
}

qreal TransformProperties::getAspect() const
{
    return mAspect;
}

void TransformProperties::setAspect(qreal newAspect)
{
    mAspect = newAspect;
}

qreal TransformProperties::getRotate() const
{
    return mRotate;
}

void TransformProperties::setRotate(qreal newRotate)
{
    mRotate = newRotate;
}

const QRectF &TransformProperties::getCrop() const
{
    return mCrop;
}

void TransformProperties::setCrop(const QRectF &newCrop)
{
    mCrop = newCrop;
}
