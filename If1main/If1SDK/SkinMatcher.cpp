#include "SkinMatcher.h"

#include <QImage>
#include <QLine>

#include <Rectangle.h>

SkinMatcher::SkinMatcher(void)
    : RegionScale(0)
{
    regions_ << QPair<QString,QRectF>("Forehead",   RectangleF(QSizeF(1.0,0.25),   QPointF(0.0,-1.0)))
             << QPair<QString,QRectF>("LeftCheek",  RectangleF(QSizeF(0.25,0.25),  QPointF(-0.5,0.0)))
             << QPair<QString,QRectF>("Nose",       RectangleF(QSizeF(0.25,0.75),  QPointF(0.0,-0.5)))
             << QPair<QString,QRectF>("RightCheek", RectangleF(QSizeF(0.25,0.25),  QPointF(0.5,0.0)));
    clear();
}

void SkinMatcher::add(const QString & name, SkinMatchProperties * props)
{
    if (properties_.contains(name))
    {
        SkinMatchProperties * oldProp = properties_.take(name);
        delete oldProp;
    }

    if (props)
        properties_.insert(name, props);
}

void SkinMatcher::clear(void)
{
    rgbAccumulator = AnyColor(RGB);
    colorValue = AnyColor(RGB);
}

AnyColor SkinMatcher::skinColor(void) const
{
    return colorValue;
}

QList<QRect> SkinMatcher::normalFaceRectagles(const Eyes eyes)
{
    QList<QRect> rects;
    QListIterator< QPair<QString,QRectF> > it(regions_);
    while (it.hasNext())
    {
        QRectF rc = it.next().second;
        RectangleF rectF(rc);
        rectF.scale(eyes.distance());
        rectF.translate(eyes.noseTip());
        if (RegionScale)
            rectF.expand((qreal)RegionScale / 100.0);
        QRect r(QPoint(rectF.left(), rectF.top()), QSize(rectF.width(), rectF.height()));
        rects.append(r);
    }
    return rects;
}

bool SkinMatcher::addNormalFaceImage(const QImage & image, const Eyes eyes)
{
    QList<QRect> rects = normalFaceRectagles(eyes);

    foreach(QRect rect, rects)
    {
        unsigned r = rect.right(), b = rect.bottom();
        for (unsigned y = rect.top(); y <= b; ++y)
            for (unsigned x = rect.left(); x <= r; ++x)
            {
            QRgb pixel = image.pixel(x, y);
            rgbAccumulator.add(AnyColor(pixel));
        }
    }

    colorValue = rgbAccumulator.average();
    return true;
} // addNormalFaceImage()

int SkinMatcher::bestConfidence(void) const
{
    unsigned bestConf = 0;
    foreach(SkinMatchProperties * prop, properties_)
    {
        unsigned conf = prop->confidence(colorValue);
        if (conf > bestConf)
            bestConf = conf;
    }
    return bestConf;
} // bestConfidence()

QString SkinMatcher::bestMatch(void) const
{
    unsigned bestConf = 0;
    QString bestName;

    foreach(SkinMatchProperties * prop, properties_)
    {
        unsigned conf = prop->confidence(colorValue);
        if (conf > bestConf && conf > prop->minConfidence())
            bestConf = conf, bestName = prop->name();
    }

    return bestName;
} // bestMatch()

AnyColor SkinMatcher::targetColor(const QString & name) const
{
    return properties_.contains(name)
            ? (properties_.value(name)->color())
            : AnyColor();
} // targetColor()
