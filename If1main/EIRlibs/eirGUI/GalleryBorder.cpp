#include "GalleryBorder.h"

#include <QtGui/QBrush>
#include <QtGui/QPen>

#include "../eirExe/Log.h"
#include "GalleryScene.h"

GalleryBorder::GalleryBorder(const QSize size,
                             const int width,
                             const QColor & color,
                             QGraphicsItem * parentItem)
    : QGraphicsRectItem(0, 0, size.width(), size.height(), parentItem)
    , width_i(width)
    , _color(color)
{
    GVIPOINTER(QGraphicsItem, parentItem);
    setZValue(GalleryScene::SelectZ);
    QGraphicsRectItem::setPen(QPen(QBrush(_color), width_i));
}

void GalleryBorder::setPen(const int width, const QColor & color)
{
    OBJPOINTER(QGraphicsScene, scene());
    width_i = width, _color = color;
    QGraphicsRectItem::setPen(QPen(QBrush(_color), width_i));
    scene()->invalidate();
}

void GalleryBorder::setWidth(const int width)
{
    OBJPOINTER(QGraphicsScene, scene());
    width_i = width;
    QGraphicsRectItem::setPen(QPen(QBrush(_color), width_i));
    scene()->invalidate();
}

void GalleryBorder::setColor(const QColor & color)
{
    OBJPOINTER(QGraphicsScene, scene());
    _color = color;
    QGraphicsRectItem::setPen(QPen(QBrush(_color), width_i));
    scene()->invalidate();
}
