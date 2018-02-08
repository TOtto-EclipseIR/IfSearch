#include "SkinMatchProperties.h"

#include <QtCore/QListIterator>

#include <Eyes.h>
//#include <IJMcore/Rectangle>

SkinMatchProperties::SkinMatchProperties(const QString & name, QObject *parent) :
    QObject(parent),
    MinConfidence(0),
    Name(name)
{
    setObjectName("SkinMatchProperties:"+Name);
}

void SkinMatchProperties::setTargetColor(QString v)
{
    TargetColor = v;
    QColor qcolor(v);
    AnyColor rgbColor(qcolor.rgb());
    matchColor = rgbColor;
}

unsigned SkinMatchProperties::confidence(const AnyColor & other)
{
    if (matchColor.isNull() || other.isNull())
        return 0;
    qreal distance = matchColor.distance(other);
    return qBound(1, (int)(1000.0 / (1.0 + distance)), 999);
}

