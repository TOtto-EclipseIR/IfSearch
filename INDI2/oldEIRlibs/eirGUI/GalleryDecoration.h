#ifndef GALLERYDECORATION_H
#define GALLERYDECORATION_H

#include <QGraphicsItemGroup>

#include "GalleryItem.h"
class GalleryParameters;

class EIRGUISHARED_EXPORT GalleryDecoration : public QGraphicsItemGroup
{
public:
    enum { Type = UserType + 3 };

public:
    GalleryDecoration(GalleryItem::Decoration decType,
                      GalleryParameters * parms,
                      GalleryItem * parentItem,
                      const QVariant & var=QVariant());
    virtual int type(void) const { return Type; }
    GalleryItem::Decoration decoration(void) const { return _decType; }

private:
    GalleryItem::Decoration _decType;
};

#endif // GALLERYDECORATION_H
