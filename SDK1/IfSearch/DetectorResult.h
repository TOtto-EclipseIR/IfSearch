#pragma once

#include <QList>
#include <QRect>

class DetectorResult
{
private:
    friend class HaarDetector;
    DetectorResult(QRect Rect)
        : k(0), sco(0), rect(Rect)
        , ptAverage(Rect.topLeft()), szAverage(Rect.size())
    { allRects.append(Rect); }

public:
    DetectorResult(int Rank=0, int Score=0, QRect Rect=QRect(),
                   QList<QRect> AllRects=QList<QRect>())
        : k(Rank), sco(Score), rect(Rect), allRects(AllRects) {}

public:
    int rank(void) { return k; }
    int score(void) const { return sco; }
    QRect rectangle(void) const { return rect; }
    QList<QRect> allRectangles(void) { return allRects; }

private:
    void addRect(QRect Rect) { rect = rect.united(Rect); allRects.append(Rect); }
    void addToAverage(QRect Rect);

private:
    int k;
    int sco;
    QRect rect;
    QPointF ptAverage;
    QSizeF szAverage;
    QList<QRect> allRects;
};

