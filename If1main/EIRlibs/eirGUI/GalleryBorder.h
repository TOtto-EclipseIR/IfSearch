#ifndef GALLERYBORDER_H
#define GALLERYBORDER_H
#include "eirGUI.h"

#include <QGraphicsRectItem>

class GalleryBorder : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 2 };

public:
    GalleryBorder(const QSize size,
                  const int width,
                  const QColor & color,
                  QGraphicsItem * parentItem);
    virtual int type(void) const { return Type; }
    void setPen(const int width, const QColor & color);
    void setWidth(const int width);
    void setColor(const QColor & color);

private:
    int width_i;
    QColor _color;
};

#endif // GALLERYBORDER_H
