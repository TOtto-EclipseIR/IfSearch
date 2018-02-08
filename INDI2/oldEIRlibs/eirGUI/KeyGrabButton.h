#ifndef KEYGRABBUTTON_H
#define KEYGRABBUTTON_H
#include "eirGUI.h"

#include <QPushButton>

#include <QtCore/QChar>
#include <QtCore/QString>

class EIRGUISHARED_EXPORT KeyGrabButton : public QPushButton
{
    Q_OBJECT

public:
    explicit KeyGrabButton(QWidget * parent=0);
    void setIcon(const QIcon & icon);
    void setText(const QString & string);
    void setGrabEnable(const bool enable=true);
    void setValidKeys(const QString & valid);
    QChar grabbedChar(void) const;

signals:
    void keyGrabbed(QChar c);

public slots:
    void toggled(bool checked);

protected:
    virtual void keyPressEvent(QKeyEvent * e);

private:
    bool grabEnable_b;
    QString valid_s;
    bool grabbed_b;
    QIcon hold_icon;
    QString hold_text;
};

#endif // KEYGRABBUTTON_H
