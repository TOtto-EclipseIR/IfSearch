#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include "eirGUI.h"

#include <QLabel>

class EIRGUISHARED_EXPORT ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget * parent=0);
    ImageLabel(const QImage defaultImage,
               QWidget * parent=0);
    void clear(void);
    void setDefault(const QImage & image);
    void set(const QImage & image);

protected:
    virtual void mousePressEvent(QMouseEvent * event);

signals:
    void clicked(void);

public slots:

private:
    QPixmap default_pixmap;
};

#endif // IMAGELABEL_H
