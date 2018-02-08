#ifndef IOWIDGET_H
#define IOWIDGET_H
#include "eirGUI.h"

#include <QTextEdit>

class QIODevice;

class EIRGUISHARED_EXPORT IOWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit IOWidget(QIODevice * device,
                      QWidget * parent=0);

signals:

public slots:
    void readLines(void);

private:
    QIODevice * _ioDevice;
};

#endif // IOWIDGET_H
