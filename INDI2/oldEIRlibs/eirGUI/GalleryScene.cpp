#include "GalleryScene.h"

#include "Gallery.h"

#include "../eirExe/Log.h"

GalleryScene::GalleryScene(Gallery * parent)
    : QGraphicsScene(parent)
    , _gallery(parent)
{
    OBJPOINTER(Gallery, _gallery);
}
