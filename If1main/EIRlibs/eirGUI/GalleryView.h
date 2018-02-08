#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include <QtCore/QPointF>
#include <QtGui/QGraphicsView>

class GalleryScene;

class GalleryView : public QGraphicsView
{
    Q_OBJECT
    friend class GalleryViewport;

public:
    explicit GalleryView(GalleryScene * scene=0);

protected:

signals:

public slots:

};

#endif // GALLERYVIEW_H
