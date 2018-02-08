#include "GalleryDecoration.h"

#include <QtGui/QGraphicsRectItem>

#include "../eirExe/Log.h"
#include "GalleryItem.h"
#include "GalleryParameters.h"
#include "GalleryScene.h"

GalleryDecoration::GalleryDecoration(GalleryItem::Decoration decType,
                                     GalleryParameters * parms,
                                     GalleryItem * parentItem,
                                     const QVariant & var)
    : QGraphicsItemGroup(parentItem)
    , _decType(decType)
{
    GVIPOINTER(QGraphicsItem, parentItem);
    QRectF rect(parentItem->boundingRect());

    switch (_decType)
    {
    case GalleryItem::Selection:
    {
        int width = parms->getBorderWidth();
        QColor color = parms->getSelectColor();
        QGraphicsRectItem * border_item = new QGraphicsRectItem(rect, this);
        GVIPOINTER(QGraphicsRectItem, border_item);
        border_item->setZValue(GalleryScene::SelectZ);
        border_item->setPen(QPen(QBrush(color), width));
    }
        break;

    case GalleryItem::YinAndYang:
    {
        int width = parms->getBorderWidth();
        QColor color = var.toBool()
                        ? parms->getYinColor()
                        : parms->getYangColor();
        QGraphicsRectItem * border_item = new QGraphicsRectItem(rect, this);
        GVIPOINTER(QGraphicsRectItem, border_item);
        border_item->setZValue(GalleryScene::YinYangZ);
        border_item->setPen(QPen(QBrush(color), width));
    }
        break;

    case GalleryItem::Score:
    {
        float f = var.toFloat();
        if (f > 1.0) f /= 1000.0;
        int border_width = parms->getBorderWidth();
        int scoreBorder_width = parms->getScoreBorderWidth();
        int score_width = parms->getScoreWidth();

        QRectF scoreBorder_rect(QPointF(rect.left() + border_width / 2,
                                        rect.bottom() - border_width / 2 - scoreBorder_width),
                                QSize(rect.width() - 2 * (border_width / 2),
                                      border_width));
        QRectF score_rect(QPointF(scoreBorder_rect.left()   + (scoreBorder_width - score_width) / 2,
                                  scoreBorder_rect.top()    + (scoreBorder_width - score_width) / 2),
                          QSize(f * (scoreBorder_rect.width() - (scoreBorder_width - score_width)),
                                score_width));
        QRectF back_rect(QPointF(score_rect.right(), score_rect.top()),
                         QSize((1.0 - f) * (scoreBorder_rect.width() - (scoreBorder_width - score_width)),
                               score_width));

        QGraphicsRectItem * scoreBorder_item = new QGraphicsRectItem(scoreBorder_rect, this);
        GVIPOINTER(QGraphicsRectItem, scoreBorder_item);
        scoreBorder_item->setBrush(QBrush(parms->getScoreBorderColor()));
        scoreBorder_item->setZValue(GalleryScene::ScoreBorderZ);

        QGraphicsRectItem * score_item = new QGraphicsRectItem(score_rect, this);
        GVIPOINTER(QGraphicsRectItem, score_item);
        score_item->setBrush(QBrush(parms->getScoreColor()));
        score_item->setZValue(GalleryScene::ScoreZ);

        QGraphicsRectItem * back_item = new QGraphicsRectItem(back_rect, this);
        GVIPOINTER(QGraphicsRectItem, back_item);
        back_item->setBrush(QBrush(parms->getScoreBackground()));
        back_item->setZValue(GalleryScene::ScoreZ);
    }
        break;

    case GalleryItem::Title:
    {
        int border_width = parms->getBorderWidth();
        int title_width = parms->getTitleWidth();
        QRectF back_rect(QPointF(rect.left() + border_width / 2,
                                  rect.top()  + border_width / 2),
                         QSize(rect.width() - 2 * (border_width / 2),
                                title_width));

        QGraphicsRectItem * back_item = new QGraphicsRectItem(back_rect, this);
        GVIPOINTER(QGraphicsRectItem, back_item);
        back_item->setBrush(QBrush(parms->getTitleBackground()));
        back_item->setZValue(GalleryScene::TitleBackgroundZ);

        QGraphicsSimpleTextItem * title_item = new QGraphicsSimpleTextItem(back_item);
        GVIPOINTER(QGraphicsSimpleTextItem, title_item);
        title_item->moveBy(back_rect.left(), back_rect.top());
        title_item->setFont(QFont("Helvetica", title_width-4));
        title_item->setBrush(QBrush(parms->getTitleColor()));
        title_item->setZValue(GalleryScene::TitleZ);
        title_item->setText(var.toString());
    }
        break;

    case GalleryItem::Chip:
    {
        int border_width = parms->getBorderWidth();
        int chip_width = 2 * parms->getTitleWidth();
        QRectF chip_rect(QPointF(rect.left() + border_width / 2,
                                 rect.bottom() - border_width / 2 - chip_width),
                         QSize(chip_width, chip_width));

        QGraphicsRectItem * back_item = new QGraphicsRectItem(chip_rect, this);
        GVIPOINTER(QGraphicsRectItem, back_item);
        back_item->setBrush(QBrush(Qt::yellow));
        back_item->setZValue(GalleryScene::TitleBackgroundZ);

        QGraphicsSimpleTextItem * title_item = new QGraphicsSimpleTextItem(back_item);
        GVIPOINTER(QGraphicsSimpleTextItem, title_item);
        title_item->moveBy(chip_rect.left(), chip_rect.top());
        title_item->setFont(QFont("Helvetica", chip_width-4));
        title_item->setBrush(QBrush(Qt::black));
        title_item->setZValue(GalleryScene::TitleZ);
        title_item->setText(var.toString());
    }
        break;
    }
}
