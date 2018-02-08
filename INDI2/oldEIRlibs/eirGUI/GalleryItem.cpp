#include "GalleryItem.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "../eirExe/Log.h"

#include "Gallery.h"
#include "GalleryDecoration.h"
#include "GalleryBorder.h"
#include "GalleryParameters.h"
#include "GalleryScene.h"

bool GalleryItem::yinAndYang_b = true;

GalleryItem::GalleryItem(Gallery * gallery,
                         const QImage & image,
                         const QString & imageId,
                         const QSize size)
    : QGraphicsPixmapItem(0)
    , _gallery(gallery)
    , _id(imageId)
    , _image(image)
{
    OBJPOINTER(Gallery, _gallery);
    _pixmap = QPixmap::fromImage(image.scaled(size));
    setPixmap(_pixmap);
    setZValue(GalleryScene::PixmapZ);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

GalleryParameters * GalleryItem::parms(void)
{
    OBJPOINTER(Gallery, _gallery);
    return _gallery->parms();
}

void GalleryItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    OBJPOINTER(Gallery, _gallery);
    qDebug() << "GalleryItem::mousePressEvent()" << event->button();
    if (Qt::LeftButton == event->button())
        _gallery->leftClicked(this);
    else if (Qt::RightButton == event->button())
        _gallery->rightClicked(this);
}

QImage GalleryItem::image(void) const
{
    return _image;
}

QString GalleryItem::id(void) const
{
    return _id;
}

void GalleryItem::addDecoration(const Decoration dec,
                                QVariant var)
{
    if (YinAndYang == dec)
        var.setValue(yinAndYang_b);
    GalleryDecoration * gd = new GalleryDecoration(dec, parms(), this, var);
    decoration_list.append(gd);
}

void GalleryItem::removeDecoration(const Decoration dec)
{
    foreach (GalleryDecoration * gd, decoration_list)
    {
        GVIPOINTER(GalleryDecoration, gd);
        if (dec == gd->decoration())
        {
            decoration_list.removeOne(gd);
            delete gd;
        }
    }
}

void GalleryItem::toggleYinAndYang(void)
{
    yinAndYang_b = ! yinAndYang_b;
}

bool GalleryItem::isYinOrYang(void)
{
    return yinAndYang_b;
}
