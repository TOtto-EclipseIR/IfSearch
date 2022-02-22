#include <DetectorResult.h>

void DetectorResult::addToAverage(QRect Rect)
{
    allRects.append(Rect);
    ptAverage.setX(ptAverage.x() + Rect.x());
    ptAverage.setY(ptAverage.y() + Rect.y());
    szAverage.setWidth(szAverage.width() + Rect.width());
    szAverage.setHeight(szAverage.height() + Rect.height());
    qreal n = allRects.size();
    rect.setX(ptAverage.x() / n);
    rect.setY(ptAverage.y() / n);
    rect.setWidth(szAverage.width() / n);
    rect.setHeight(szAverage.height() / n);
} // addToAverate()

