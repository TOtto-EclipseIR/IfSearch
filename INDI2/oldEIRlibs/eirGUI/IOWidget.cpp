#include "IOWidget.h"

#include <QtDebug>
#include <QtCore/QIODevice>
#include <QtGui/QFont>

#include "../eirExe/Log.h"

IOWidget::IOWidget(QIODevice * device,
                   QWidget *parent)
    : QTextEdit(parent)
    , _ioDevice(device)
{
    qDebug() << "IOWidget c'tor";
    setObjectName("IOWidget");
    QFont font("Courier");
    font.setFixedPitch(true);
    QTextEdit::setFont(font);
    QTextEdit::setFocusPolicy(Qt::NoFocus);

    CONNECT(_ioDevice, SIGNAL(readyRead()),
            this, SLOT(readLines()));
}

void IOWidget::readLines(void)
{
    while (_ioDevice->canReadLine())
        QTextEdit::append(QString(_ioDevice->readLine()).simplified());
}
