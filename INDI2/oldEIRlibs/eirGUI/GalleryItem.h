#ifndef GALLERYITEM_H
#define GALLERYITEM_H
#include "eirGUI.h"

#include <QtGui/QGraphicsPixmapItem>

#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

class Gallery;
class GalleryDecoration;
class GalleryParameters;
class GalleryBorder;

class EIRGUISHARED_EXPORT GalleryItem : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 1 };
    enum Decoration
    {
        Selection = 1,
        YinAndYang,
        Score,
        Title,
        Chip,
    };


public:
    GalleryItem(Gallery * gallery,
                const QImage & image,
                const QString & imageId,
                const QSize size);
    virtual int type(void) const { return Type; }
    QImage image(void) const;
    QString id(void) const;
    void addDecoration(Decoration dec,
                       QVariant var=QVariant());
    void removeDecoration(Decoration dec);
    static void toggleYinAndYang(void);
    static bool isYinOrYang(void);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

private:
    GalleryParameters * parms(void);

private:
    Gallery * _gallery;
    QString _id;
    QImage _image;
    QPixmap _pixmap;
    QList<GalleryDecoration *> decoration_list;
    static bool yinAndYang_b;
};

typedef QList<GalleryItem *> GalleryItemList;

#endif // GALLERYITEM_H
