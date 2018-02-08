#ifndef GALLERYPARAMETERS_H
#define GALLERYPARAMETERS_H
#include "eirGUI.h"

#include "../eirCore/VariableGroup.h"

#include <QtCore/QSize>
#include <QtGui/QColor>

#define GALLERYPARAMETERS_GROUPVARIABLES(ITD) \
    ITD(ThumbSize, QSize, QSize(96,96)) \
    ITD(MinThumbs, QSize, QSize(5,2)) \
    ITD(Padding, int, 8) \
    ITD(BorderWidth, int, 5) \
    ITD(SelectColor, QColor, QColor(Qt::yellow)) \
    ITD(YinColor, QColor, QColor(Qt::darkBlue)) \
    ITD(YangColor, QColor, QColor(Qt::darkRed)) \
    ITD(ScoreBorderWidth, int, 10) \
    ITD(ScoreBorderColor, QColor, QColor(Qt::darkGray)) \
    ITD(ScoreWidth, int, 6) \
    ITD(ScoreColor, QColor, QColor(Qt::white)) \
    ITD(ScoreBackground, QColor, QColor(Qt::blue)) \
    ITD(TitleColor, QColor, QColor(Qt::white)) \
    ITD(TitleBackground, QColor, QColor(Qt::darkGray)) \
    ITD(TitleWidth, int, 10) \
    ITD(SelectionMode, int, 2) \
    ITD(MaxItems, int, 0) \
    ITD(Flags, int, 0x00000011) \


class EIRGUISHARED_EXPORT GalleryParameters : public VariableGroup
{
public:
    DECLARE_GROUPVARIABLES(GALLERYPARAMETERS_GROUPVARIABLES);
    explicit GalleryParameters(const QString & section=QString());
    QPointF position(const int index,
                     const QSize viewSize);

signals:

public slots:

};

#endif // GALLERYPARAMETERS_H
