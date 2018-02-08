#include "GalleryParameters.h"

#include <QtCore/QPointF>

#include "../eirCore/eirCore.h"

GalleryParameters::GalleryParameters(const QString & section) :
    VariableGroup(section)
{
    DEFINE_GROUPVARIABLES_CTORS(GALLERYPARAMETERS_GROUPVARIABLES);
}

QPointF GalleryParameters::position(const int index,
                                    const QSize viewSize)
{
    int pad = getPadding();
    QSize sz = getThumbSize();
    int width = viewSize.width() / (pad + sz.width());
    return QPointF(pad + (index % width) * (sz.width()  + pad),
                   pad + (index / width) * (sz.height() + pad));
}
