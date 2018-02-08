#include "IdPane.h"
#include "MainWindow.h"

#include <QtDebug>
#include <QtCore/QTimer>
#include <QtGui/QFont>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>

#include <eirExe/Log.h>
#include <eirGUI/KeyGrabButton.h>


IdPane::IdPane(MainWindow * parent)
    : QWidget(parent)
    , mw_ptr(parent)
    , _hLayout(new QHBoxLayout)
    , _button(new KeyGrabButton)
    , left_text(new QLineEdit)
    , _progress(new QProgressBar)
    , base_icon(":/icon/GradIcon.ico")
    , reset_timer(new QTimer(this))
{
    qDebug() << "IdPane c'tor";
    QLabel temp_label("TokenPointIR", this);
    temp_label.setFont(_button->font());
    _button->setFixedSize(32, 32);
    _button->setIcon(base_icon);
    left_text->setAutoFillBackground(true);
    left_text->setFont(_button->font());
    left_text->setText("TokenPointIR");
    left_text->setMaximumHeight(temp_label.height()+2);
    left_text->setReadOnly(true);
    left_text->setBackgroundRole(QPalette::Window);
    _progress->setVisible(false);

    _hLayout->addWidget(_button);
    _hLayout->addWidget(left_text);
    _hLayout->addWidget(_progress);
    this->setLayout(_hLayout);

    QPixmap red(20, 20);
    red.fill(Qt::red);
    error_icon.addPixmap(red);

    CONNECT(reset_timer, SIGNAL(timeout()),
            this, SLOT(reset()));
}

MainWindow * IdPane::mainw(void) const
{
    OBJPOINTER(MainWindow, mw_ptr);
    return mw_ptr;
}

void IdPane::reset(void)
{
    _button->setIcon(base_icon);
    left_text->setBackgroundRole(QPalette::Base);
    left_text->setText("TokenPointIR");
    left_text->setVisible(true);
    _progress->setVisible(false);
}

void IdPane::resetIn(const int msec)
{
    reset_timer->stop();
    if (msec)
        reset_timer->start(msec);
}

void IdPane::progress(const int maximum,
              const int minimum,
              const int value)
{
    _progress->setMaximum(maximum);
    _progress->setMinimum(minimum);
    _progress->setValue(value);
    _progress->setVisible(true);
    _progress->update();
}

void IdPane::progress(const int maximum,
                      const int minimum)
{
    _progress->setMaximum(maximum);
    _progress->setMinimum(minimum);
    _progress->setValue(minimum);
    _progress->setVisible(true);
    _progress->update();
}



void IdPane::progress(const int value)
{
    _progress->setValue(value);
    _progress->update();
}

void IdPane::hideProgress(void)
{
    _progress->setVisible(false);
}

void IdPane::setId(const QString & id)
{
    left_text->setText(id);
}

QString IdPane::token(void) const
{
    return mainw()->tokenString(_button->grabbedChar());
}

void IdPane::setToken(const QString tokenString)
{
    QChar c(mainw()->tokenChar(tokenString));
    if (c.isNull())
    {
        _button->setText(QString());
        _button->setIcon(base_icon);
    }
    else
    {
        _button->setIcon(QIcon());
        _button->setText(c);
    }
}

void IdPane::message(const QString & msg)
{
    _button->setIcon(base_icon);
    left_text->setText(msg);
}

void IdPane::error(const QString & msg)
{
    reset();
    _button->setIcon(error_icon);
    left_text->setText(msg);
    left_text->setBackgroundRole(QPalette::AlternateBase);
}

void IdPane::edit(void)
{
    _button->setCheckable(true);
    _button->setChecked(false);
    _button->setGrabEnable(true);
    left_text->clear();
    left_text->setReadOnly(false);
    left_text->setVisible(true);
    _progress->setVisible(false);
}

void IdPane::endEdit(void)
{
    _button->setChecked(false);
    _button->setCheckable(false);
    _button->setGrabEnable(false);
    left_text->setReadOnly(true);
}

QString IdPane::personId(void) const
{
    return left_text->text();
}

