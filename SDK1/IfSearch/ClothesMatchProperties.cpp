#include "ClothesMatchProperties.h"
#include "CharacteristicColor.h"

#include <QtCore/QRect>

#include <Eyes.h>
#include <Rectangle.h>


ClothesMatchProperties::ClothesMatchProperties(QObject *parent) :
    QObject(parent)
{
    DEFINE_QPROPERTIES_CTORS(CLOTHESMATCH_QPROPERTIES);
    setObjectName("ClothesMatchProperties");
}

qreal ClothesMatchProperties::shoulder(void) const
{
    int s = getShoulder();
    return (qreal)(s ? s : 128) / 64.0;
}

qreal ClothesMatchProperties::waist(void) const
{
    int s = getWaist();
    return (qreal)(s ? s : 512) / 64.0;
}

qreal ClothesMatchProperties::width(void) const
{
    int s = getWidth();
    return (qreal)(s ? s : 256) / 64.0;
}

qreal ClothesMatchProperties::ankle(void) const
{
    int s = getAnkle();
    return (qreal)(s ? s : 1536) / 64.0;
}

qreal ClothesMatchProperties::underCrop(void) const
{
    int s = getUnderCrop();
    return (qreal)(s ? s : 80) / 100.0;
}

QRect ClothesMatchProperties::upperRect(const Eyes eyes) const
{
    qreal eyeDistance = eyes.distance();
    QPoint noseTip = eyes.noseTip();
    unsigned t = noseTip.y() + shoulder() * eyeDistance;
    unsigned b = noseTip.y() + waist() * eyeDistance;
    unsigned l = noseTip.x() - width() * eyeDistance / 2.0;
    unsigned r = noseTip.x() + width() * eyeDistance / 2.0;
    QRect rc(QPoint(l, t), QPoint(r, b));
    Rectangle crop(rc);
    crop.expand(underCrop());
    return crop;
} // upperRect()

QRect ClothesMatchProperties::lowerRect(const Eyes eyes) const
{
    qreal eyeDistance = eyes.distance();
    QPoint noseTip = eyes.noseTip();
    unsigned t = noseTip.y() + waist() * eyeDistance;
    unsigned b = noseTip.y() + ankle() * eyeDistance;
    unsigned l = noseTip.x() - width() * eyeDistance / 2.0;
    unsigned r = noseTip.x() + width() * eyeDistance / 2.0;
    QRect rc(QPoint(l, t), QPoint(r, b));
    Rectangle crop(rc);
    crop.expand(underCrop());
    return crop;
} // lowerRect()

CharacteristicColor ClothesMatchProperties::upperCharacteristicColor(void) const
{
    return CharacteristicColor(QColor(getUpperColor()));
} // upperCharacteristicColor()

CharacteristicColor ClothesMatchProperties::lowerCharacteristicColor(void) const
{
    return CharacteristicColor(QColor(getLowerColor()));
} // lowerCharacteristicColor()
