#ifndef GALLERY_H
#define GALLERY_H
#include "eirGUI.h"

#include <QObject>

#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtGui/QBoxLayout>
class QAction;
class QWidget;

#include "GalleryItem.h"
class GalleryParameters;
class GalleryScene;
class GalleryView;

class EIRGUISHARED_EXPORT Gallery : public QObject
{
    Q_OBJECT

public:
    enum SelectionMode
    {
        SelectNone = 0,
        SelectSingle,
        SelectMultiple,
    };

    enum Action
    {
        ClearSelection = 1,
        RemoveUnselected,
    };

    enum Flag
    {
        ProtectSelected     = 0x00000001,
        RemoveSelected      = 0x00000002,
        RightClickRemove    = 0x00000010,
        RightClickMask      = 0x000000F0,
    };

public:
    explicit Gallery(GalleryParameters * parms,
                     QObject * parent=0);
    GalleryParameters * parms(void) const;
    QWidget * widget(void) const;
    QWidget * navigator(const QBoxLayout::Direction direction=QBoxLayout::TopToBottom);
    QSize viewSize(void);
    QSize viewThumbs(void);
    QAction * action(Action act, QObject * parent=0);
    GalleryItem * add(const QImage & image,
                      const QString & imageId);
    int add(GalleryItemList items);
    GalleryItemList items(void) const;
    int size(void) const;
    bool contains(const QString & imageId) const;
    void remove(GalleryItem * item);
    void remove(GalleryItemList items);
    void showPosition(const int index);
    SelectionMode selectionMode(void) const;
    void setSelectionMode(const SelectionMode mode);
    void moveSelection(const bool top=true);
    bool isSelected(GalleryItem * item);
    void select(GalleryItem * item);
    void selectPosition(const int index);
    void select(GalleryItemList items);
    void unselect(GalleryItem * item);
    void selectAdd(GalleryItem * item);
    GalleryItem * selectedItem(void) const;
    GalleryItemList selectedList(void) const;
    void leftClicked(GalleryItem * item);
    void rightClicked(GalleryItem * item);

signals:
    void added(int index, int itemCount);
    void selected(GalleryItem * item);
    void unselected(GalleryItem * item);
    void removed(GalleryItem * item);
    void hasSelected(bool oneOrMore);

public slots:
    void clear(void);
    void clearSelection(void);
    void removeUnselected(void);

private slots:
    void scrollAction(int action);
    void scrollRange(int min, int max);
    void scrollValue(int value);

private:
    void reposition(void);

private:
    GalleryParameters * _parms;
    GalleryScene * _scene;
    GalleryView * _view;
    GalleryItemList item_list;
    GalleryItemList selected_list;
    GalleryItem * selected_item;
    SelectionMode _mode;
    bool scrollSticky_bool;
};

#endif // GALLERY_H
