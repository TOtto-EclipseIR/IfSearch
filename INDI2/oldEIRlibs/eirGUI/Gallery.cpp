#include "Gallery.h"

#include <QtDebug>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtGui/QScrollBar>

#include "../eirExe/Log.h"
#include "../eirGUI/QQIcon.h"
#include "GalleryItem.h"
#include "GalleryParameters.h"
#include "GalleryScene.h"
#include "GalleryView.h"

Gallery::Gallery(GalleryParameters * parms,
                 QObject * parent)
    : QObject(parent)
    , _parms(parms)
    , _scene(new GalleryScene(this))
    , _view(new GalleryView(_scene))
    , selected_item(0)
    , _mode(SelectMultiple)
    , scrollSticky_bool(true)
{
    qDebug() << "Gallery c'tor";
    OBJPOINTER(GalleryScene, _scene);
    OBJPOINTER(GalleryView, _view);
    setObjectName("Gallery");
    setSelectionMode(SelectionMode(_parms->getSelectionMode()));

    QSize thumbSize(_parms->getThumbSize());
    QSize minThumbs(_parms->getMinThumbs());
    int pad = _parms->getPadding();
    _view->setMinimumSize((thumbSize.width()  + 2 * pad) * minThumbs.width(),
                          (thumbSize.height() + 2 * pad) * minThumbs.height());

    QScrollBar * bar = _view->verticalScrollBar();
    OBJPOINTER(QScrollBar, bar);
    CONNECT(bar, SIGNAL(actionTriggered(int)),
            this, SLOT(scrollAction(int)));
    CONNECT(bar, SIGNAL(rangeChanged(int,int)),
            this, SLOT(scrollRange(int,int)));
    CONNECT(bar, SIGNAL(valueChanged(int)),
            this, SLOT(scrollValue(int)));
}

GalleryParameters * Gallery::parms(void) const
{
    return _parms;
}


QWidget * Gallery::widget(void) const
{
    OBJPOINTER(GalleryView, _view);
    return (QWidget *)_view;
}


GalleryItem * Gallery::add(const QImage & image,
                           const QString & imageId)
{
    OBJPOINTER(GalleryScene, _scene);
    OBJPOINTER(GalleryView, _view);

    int f = _parms->getFlags();
    int n = _parms->getMaxItems();
    int x = 0;
    while (n && item_list.size() > n-1 && x < item_list.size())
    {
        GalleryItem * gi = item_list.at(x);
        GVIPOINTER(GalleryItem, gi);
        if (f & ProtectSelected)
            if (selected_item == gi || selected_list.contains(gi))
            {
                ++x;
                continue;
            }
        remove(gi);
    }

    int index = item_list.size();
    GalleryItem * gi = new GalleryItem(this, image, imageId,
                                       _parms->getThumbSize());
    GVIPOINTER(GalleryItem, gi);
    _scene->addItem(gi);
    gi->setPos(_parms->position(index, viewSize()));
    item_list.append(gi);
    emit added(index, item_list.size());

    return gi;
}

int Gallery::add(GalleryItemList items)
{
    int count = 0;
    foreach (GalleryItem * gi, items)
    {
        GVIPOINTER(GalleryItem, gi);
        add(gi->image(), gi->id());
        ++count;
    }
    return count;
}

GalleryItemList Gallery::items(void) const
{
    return item_list;
}

int Gallery::size(void) const
{
    return item_list.size();
}

bool Gallery::contains(const QString & imageId) const
{
    foreach (GalleryItem * gi, item_list)
    {
        GVIPOINTER(GalleryItem, gi);
        if (gi->id() == imageId)
            return true;
    }
    return false;
}

void Gallery::remove(GalleryItem * item)
{
    OBJPOINTER(GalleryScene, _scene);
    GVIPOINTER(GalleryItem, item);
    unselect(item);
    item_list.removeAll(item);
    _scene->removeItem(item);
    reposition();
    /* qDebug() << objectName()
             << "Gallery::remove()"
             << item->id(); */
    emit removed(item);
    delete item;
}

void Gallery::remove(GalleryItemList items)
{
    OBJPOINTER(GalleryScene, _scene);
    foreach (GalleryItem * item, items)
    {
        item_list.removeAll(item);
        _scene->removeItem(item);
        emit removed(item);
        delete item;
    }
    reposition();
}

void Gallery::showPosition(int index)
{
    OBJPOINTER(GalleryView, _view);
    if (index >= 0 && index < item_list.size())
    {
        scrollSticky_bool = false;
        _view->ensureVisible(item_list.at(index));
    }
    else if (-1 == index)
    {
        scrollSticky_bool = true;
        _view->ensureVisible(item_list.last());
    }
}

void Gallery::reposition(void)
{
    OBJPOINTER(GalleryView, _view);
    int n = item_list.size();
    QSize sz = viewSize();
    for (int x = 0; x < n; ++x)
    {
        GalleryItem * gi = item_list.at(x);
        GVIPOINTER(GalleryItem, gi);
        gi->setPos(_parms->position(x, sz));
    }
}

QAction * Gallery::action(Action act, QObject * parent)
{
    QAction * result = new QAction(parent);

    switch (act)
    {
    case ClearSelection:
        result->setText("Clear Selection");
        result->setIconText("Unselect");
        result->setToolTip("Clear Selection");
        result->setIcon(QQIcon("item-select-unselect-all"));
        CONNECT(result, SIGNAL(triggered()),
                this, SLOT(clearSelection()));
        break;

    case RemoveUnselected:
        result->setText("Remove Unselected");
        result->setIconText("Remove");
        result->setToolTip("Remove Unselected");
        result->setIcon(QQIcon("item-select-remove-unselected"));
        CONNECT(result, SIGNAL(triggered()),
                this, SLOT(removeUnselected()));
        break;
    }

    return result;
}

QSize Gallery::viewSize(void)
{
    OBJPOINTER(GalleryView, _view);
    return _view->viewport()->contentsRect().size();
}

QSize Gallery::viewThumbs(void)
{
    QSize thumbSize(_parms->getThumbSize());
    int pad = _parms->getPadding();
    int viewCols = viewSize().width()
                    / (thumbSize.width()  + pad);
    int viewRows = viewSize().height()
                    / (thumbSize.height() + pad);
    return QSize(viewCols, viewRows);
}

void Gallery::leftClicked(GalleryItem * item)
{
    GVIPOINTER(GalleryItem, item);
    if (selected_item) GVIPOINTER(GalleryItem, selected_item);
    switch (_mode)
    {
    case SelectNone:
        clearSelection();
        break;

    case SelectSingle:
        select((selected_item == item) ? 0 : item);
        break;

    case SelectMultiple:
        if (selected_list.contains(item))
            unselect(item);
        else
            selectAdd(item);
        break;
    }
}

void Gallery::rightClicked(GalleryItem * item)
{
    GVIPOINTER(GalleryItem, item);
    int f = RightClickMask & _parms->getFlags();
    switch (f)
    {
    case RightClickRemove:
        remove(item);
        break;
    }
}

Gallery::SelectionMode Gallery::selectionMode(void) const
{
    return _mode;
}

void Gallery::setSelectionMode(const SelectionMode mode)
{
    qDebug() << "Gallery::setSelectionMode()" << mode;
    clearSelection();
    _mode = mode;
}

void Gallery::clearSelection(void)
{
    qDebug() << "Gallery::clearSelection()";
    if (selected_item)
    {
        GVIPOINTER(GalleryItem, selected_item);
        unselect(selected_item);
    }
    while ( ! selected_list.isEmpty())
    {
        GalleryItem * gi = selected_list.takeLast();
        GVIPOINTER(GalleryItem, gi);
        gi->removeDecoration(GalleryItem::Selection);
        emit unselected(gi);
    }
    // qDebug() << "emit hasSelected(false)";
    emit hasSelected(false);
}

void Gallery::moveSelection(const bool top)
{
    if (selected_item)
    {
        GVIPOINTER(GalleryItem, selected_item);
        item_list.removeAll(selected_item);
    }
    foreach (GalleryItem * gi, selected_list)
    {
        GVIPOINTER(GalleryItem, gi);
        item_list.removeAll(gi);
    }

    if (top)
    {
        if (selected_item) item_list.prepend(selected_item);
        foreach (GalleryItem * gi, selected_list)
            item_list.prepend(gi);
        reposition();
        showPosition(0);
    }
    else
    {
        if (selected_item) item_list.append(selected_item);
        item_list.append(selected_list);
        reposition();
        showPosition(-1);
    }

}

void Gallery::select(GalleryItemList items)
{
    foreach (GalleryItem * gi, items)
    {
        GVIPOINTER(GalleryItem, gi);
        if ( ! isSelected(gi))
            select(gi);
    }
}

bool Gallery::isSelected(GalleryItem * item)
{
    GVIPOINTER(GalleryItem, item);
    if (selected_item == item) return true;
    if (selected_list.contains(item)) return true;
    return false;
}

void Gallery::selectPosition(const int index)
{
    if (index < 0 || index >= item_list.size()) return;
    GalleryItem * gi = item_list.at(index);
    GVIPOINTER(GalleryItem, gi);
    /* qDebug() << "Gallery::selectPosition()" << index
             << gi->id() << gi->image().size(); */
    if (SelectSingle == _mode)
        select(gi);
    else
        selectAdd(gi);
}

void Gallery::select(GalleryItem * item)
{
    if (item)
    {
        GVIPOINTER(GalleryItem, item);
        //qDebug() << "Gallery::select()" << item->id();
        if (selected_item && selected_item != item)
        {
            GVIPOINTER(GalleryItem, selected_item);
            //qDebug() << "unselect" << selected_item->id();
            unselect(selected_item);
            emit hasSelected(false);
        }
        switch (_mode)
        {
        case SelectNone:
            //qDebug() << "SelectNone";
            break;

        case SelectSingle:
            //qDebug() << "SingleSelect";
            selected_item = item;
            GVIPOINTER(GalleryItem, selected_item);
            //qDebug() << "select" << selected_item->id();
            selected_item->addDecoration(GalleryItem::Selection);
            selected_item->ensureVisible();
            emit selected(selected_item);
            emit hasSelected(true);
            break;

        case SelectMultiple:
            //qDebug() << "SelectMultiple";
            selectAdd(item);
            break;
        }
    }
    else
    {
        GVIPOINTER(GalleryItem, selected_item);
        unselect(selected_item);
    }
    //qDebug() << "Gallery::select() exit";
}

void Gallery::selectAdd(GalleryItem * item)
{
    GVIPOINTER(GalleryItem, item);
    if (selected_list.contains(item)) return;
    if (SelectMultiple == _mode)
    {
        selected_list.append(item);
        item->addDecoration(GalleryItem::Selection);
        item->ensureVisible();
        emit selected(item);
        emit hasSelected(true);
    }
}

void Gallery::unselect(GalleryItem * item)
{
    if ( ! item) return;

    GVIPOINTER(GalleryItem, item);

    if (selected_item == item)
    {
        selected_item->removeDecoration(GalleryItem::Selection);
        emit unselected(selected_item);
        emit hasSelected(false);
        selected_item = 0;
    }

    if (selected_list.contains(item))
    {
        selected_list.removeAll(item);
        item->removeDecoration(GalleryItem::Selection);
        emit unselected(item);
        emit hasSelected( ! selected_list.isEmpty());
    }
}

GalleryItem * Gallery::selectedItem(void) const
{
    return selected_item;
}

GalleryItemList Gallery::selectedList(void) const
{
    return selected_list;
}

void Gallery::clear(void)
{
    qDebug() << objectName() << "Gallery::clear() size:"
             << item_list.size();
    foreach (GalleryItem * gi, item_list)
        remove(gi);
}

void Gallery::scrollAction(int action)
{
    OBJPOINTER(GalleryView, _view);
    scrollSticky_bool = QAbstractSlider::SliderToMaximum == action;
    if (scrollSticky_bool)
    {
        QScrollBar * bar = _view->verticalScrollBar();
        if (bar)
        {
            OBJPOINTER(QScrollBar, bar);
            bar->setValue(bar->maximum());
        }
    }
}

void Gallery::scrollRange(int min, int max)
{
    TODO("Why %1 %2", min, max);
    OBJPOINTER(GalleryView, _view);
    if (scrollSticky_bool)
    {
        QScrollBar * bar = _view->verticalScrollBar();
        if (bar)
        {
            OBJPOINTER(QScrollBar, bar);
            bar->setValue(bar->maximum());
        }
    }
}

void Gallery::scrollValue(int value)
{
    OBJPOINTER(GalleryView, _view);
    QScrollBar * bar = _view->verticalScrollBar();
    if (bar)
    {
        OBJPOINTER(QScrollBar, bar);
        scrollSticky_bool = value > (bar->maximum() - bar->singleStep());
        if (scrollSticky_bool)
            bar->setValue(bar->maximum());
    }
}

void Gallery::removeUnselected(void)
{
    foreach (GalleryItem * gi, item_list)
    {
        GVIPOINTER(GalleryItem, gi);
        if (selected_item == gi || selected_list.contains(gi))
            continue;
        remove(gi);
    }
}
