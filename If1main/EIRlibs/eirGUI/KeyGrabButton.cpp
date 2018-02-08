#include "KeyGrabButton.h"

#include <QtGui/QFont>
#include <QtGui/QKeyEvent>

#include "../eirExe/Log.h"

KeyGrabButton::KeyGrabButton(QWidget * parent)
    : QPushButton(parent)
    , grabEnable_b(false)
    , valid_s("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
{
    QFont f(font());
    f.setBold(true);
    f.setPointSize(16);
    setFont(f);
    clearFocus();
    setFocusPolicy(Qt::NoFocus);
}

void KeyGrabButton::setIcon(const QIcon & icon)
{
    QPushButton::setText(QString());
    QPushButton::setIcon(icon);
}

void KeyGrabButton::setText(const QString & string)
{
    QPushButton::setIcon(QIcon());
    QPushButton::setText(string);
}

void KeyGrabButton::setGrabEnable(const bool enable)
{
    grabEnable_b = enable;
    if (grabEnable_b)
    {
        CONNECT(this, SIGNAL(toggled(bool)),
                this, SLOT(toggled(bool)));
    }
    else
    {
        DISCONNECT(this, SIGNAL(toggled(bool)),
                this, SLOT(toggled(bool)));
        clearFocus();
        setFocusPolicy(Qt::NoFocus);
        releaseKeyboard();
    }
}

void KeyGrabButton::setValidKeys(const QString & valid)
{
    valid_s = valid;
}

QChar KeyGrabButton::grabbedChar(void) const
{
    if (text().isEmpty())
        return QChar();
    else
        return text().at(0);
}

void KeyGrabButton::toggled(bool checked)
{
    if (checked)
    {
        if (grabEnable_b && ! grabbed_b)
        {
            hold_icon = icon();
            hold_text = text();
            setIcon(QIcon());
            setText(" ");
            setFocusPolicy(Qt::StrongFocus);
            grabKeyboard();
            grabbed_b = true;
        }
    }
    else
    {
        if (grabbed_b)
        {
            setIcon(hold_icon);
            setText(hold_text);
            releaseKeyboard();
            clearFocus();
            setFocusPolicy(Qt::NoFocus);
            grabbed_b = false;
        }
    }
}

void KeyGrabButton::keyPressEvent(QKeyEvent * e)
{
    int key = e->key();
    if (Qt::Key_Escape == key)
    {
        setIcon(hold_icon);
        setText(hold_text);
        releaseKeyboard();
        grabbed_b = false;
        e->accept();
        return;
    }

    QChar c(key);
    if (valid_s.contains(c, Qt::CaseInsensitive))
    {
        setText(c);
        releaseKeyboard();
        grabbed_b = false;
        e->accept();
        emit keyGrabbed(c);
        return;
    }

    e->ignore();
}
