#include "eirGUI.h"

#include <QtCore/QDir>
#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtSvg/QSvgRenderer>

#include "../eirExe/Log.h"

eirGUI::eirGUI()
{
}

QImage eirGUI::eirLogo(const QSize size)
{
    return fromSvg(":/icons/eirOnly.svg", size);
}

QImage eirGUI::fromSvg(const QString & svgFilename,
                       const QSize size)
{
    TRACE("eirGUI resources: %1", QDir(":/").entryList());
    Q_INIT_RESOURCE(eirGUI);
    TRACE("eirGUI resources: %1", QDir(":/").entryList());
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QSvgRenderer svg(svgFilename);
    QPainter painter(&image);
    svg.render(&painter);
    painter.end();
    return image;
}
