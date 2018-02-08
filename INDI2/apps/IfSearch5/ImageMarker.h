/** @file ImageMarker.h
*	Public declaration of ImageMarker class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#include <qglobal.h>

#include <QPainter>

#include <Eyes.h>

class  ImageMarker : public QPainter
{
public:
    ImageMarker(QImage * device);
    ~ImageMarker();
    void rect(QRect r, QColor color=Qt::yellow, int penWidth=0);
    void rects(QList<QRect> rl, QColor color=Qt::yellow,
               int penWidth=0, int darker=0, const qreal opacity=1.0);
    void centerLines(QRect r, QColor color=Qt::yellow, int penWidth=0);
    void horizontalLine(const QPoint pt,
                        const int length,
                        const QColor color=Qt::yellow,
                        const int penWidth=0);
    void eyes(Eyes eyes, QColor color=Qt::yellow, int penWidth=0, int length=4);
    void title(const QString text, QColor color=Qt::lightGray, QColor bgColor = Qt::darkGray, QFont font=QFont());
    void title(const QRect rect, const QString text, QColor color=Qt::lightGray, QColor bgColor = Qt::darkGray, QFont font=QFont(), Qt::AlignmentFlag align=Qt::AlignTop);
    void score(qreal value,
               int width=6,
               QColor barColor=Qt::blue,
               QColor bgColor=Qt::black,
               QColor fgColor=Qt::transparent,
               int length=0);
    void score(int value,
               int width=6,
               QColor barColor=Qt::blue,
               QColor bgColor=Qt::black,
               QColor fgColor=Qt::transparent,
               int length=0)
    { score((qreal)value / 1000.0, width, barColor, bgColor, fgColor, length); }
    void detectorScale(QSize detectorSize, QList<QSize> sizes,
                       QColor color=Qt::blue, int penWidth=0,
                       Qt::Corner corner=Qt::TopLeftCorner);

private:
}; // class Config
