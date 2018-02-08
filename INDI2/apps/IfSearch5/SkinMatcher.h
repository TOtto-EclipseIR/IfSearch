#ifndef SKINMATCHER_H
#define SKINMATCHER_H


#include "AnyColor.h"
#include "SkinMatchProperties.h"

class QImage;
class QString;
#include <QMap>

#include <Eyes.h>

class SkinMatcher
{
public:
    SkinMatcher(void);
    void add(const QString & name, SkinMatchProperties * props);
    void clear(void);
    void setRegionScale(unsigned v) { RegionScale = v; }
    unsigned regionScale(void) const { return RegionScale; }
    bool addNormalFaceImage(const QImage & image, const Eyes eyes);
    QList<QRect> normalFaceRectagles(const Eyes eyes);
    AnyColor skinColor(void) const;
    int bestConfidence(void) const;
    QString bestMatch(void) const;
    AnyColor targetColor(const QString & name) const;

private:
    QMap<QString, SkinMatchProperties *> properties_;
    QList< QPair<QString,QRectF> > regions_;
    AnyColor rgbAccumulator;
    AnyColor colorValue;
    unsigned RegionScale;
};

#endif // SKINMATCHER_H
