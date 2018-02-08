#include "EigenFaceSearchTier.h"

DEFINE_ENUMERATION(EigenFaceSearchTier, EIGENFACESEARCHTIER_ENUM);
QList<QColor> EigenFaceSearchTier::color_list =
        QList<QColor>() << QColor()             /* Unknown */
                        << QColor(Qt::yellow)   /* NoMatch */
                        << QColor("#CF0")       /* Weak */
                        << QColor("#FC0")       /* Possible */
                        << QColor(Qt::red)      /* Strong */
                        << QColor(Qt::magenta)  /* Best */
                        << QColor();            /* size */

QChar EigenFaceSearchTier::indicator(void) const
{
    return Enumeration::isValid() ? name().at(0) : QChar('?');
}

QColor EigenFaceSearchTier::color(void) const
{
    return (Enumeration::isValid())
            ? color_list[Enumeration::value()]
            : QColor();
}

QColor EigenFaceSearchTier::color(const EigenFaceSearchTier tier)
{
    return (tier.isValid()) ? color_list[tier.value()] : QColor();
}

void EigenFaceSearchTier::setColor(const EigenFaceSearchTier tier,
                                   const QColor & color)
{
    if (tier.isValid()) color_list[tier.value()] = color;
}
