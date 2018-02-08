#include "GalleryView.h"

#include <QtDebug>
#include <QtGui/QScrollBar>
#include <QtGui/QGraphicsItem>

#include "../eirCore/eirCore.h"
#include "GalleryScene.h"

GalleryView::GalleryView(GalleryScene * scene) :
    QGraphicsView(scene)
{
    setObjectName("GalleryView");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    show();
}
