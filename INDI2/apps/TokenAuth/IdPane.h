#ifndef IDPANE_H
#define IDPANE_H

#include <QWidget>

#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QPushButton>
class QHBoxLayout;
class QLineEdit;
class QTimer;
class QProgressBar;

class KeyGrabButton;

class MainWindow;

class IdPane : public QWidget
{
    Q_OBJECT

public:
    explicit IdPane(MainWindow * parent);
    void progress(const int maximum,
                  const int minimum,
                  const int value);
    void progress(const int maximum,
                  const int minimum);
    void progress(const int value);
    void hideProgress(void);
    void setId(const QString & id=QString());
    QString token(void) const;
    void setToken(const QString tokenString);
    QString personId(void) const;

signals:

public slots:
    void reset(void);
    void resetIn(const int msec);
    void message(const QString & msg);
    void error(const QString & msg);
    void edit(void);
    void endEdit(void);

private:
    MainWindow * mainw(void) const;

private:
    MainWindow * mw_ptr;
    QHBoxLayout * _hLayout;
    KeyGrabButton * _button;
    QLineEdit * left_text;
    QProgressBar * _progress;
    QIcon base_icon;
    QIcon error_icon;
    QTimer * reset_timer;
};

#endif // IDPANE_H
