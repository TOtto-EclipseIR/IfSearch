#ifndef CHECKACTION_H
#define CHECKACTION_H
#include "eirGUI.h"

#include <QtGui/QAction>

#include "../eirBase/QProperty.h"

#define CHECKACTION_QPROPERTIES(TVND) \
    TVND(QIcon,     QIcon,      CheckedIcon,          QIcon()) \
    TVND(QString,   QString,    CheckedText,          QString()) \
    TVND(QString,   QString,    CheckedIconText,      QString()) \
    TVND(QString,   QString,    CheckedToolTip,       QString()) \
    TVND(QString,   QString,    CheckedStatusTip,     QString()) \
    TVND(QIcon,     QIcon,      UncheckedIcon,        QIcon()) \
    TVND(QString,   QString,    UncheckedText,        QString()) \
    TVND(QString,   QString,    UncheckedIconText,    QString()) \
    TVND(QString,   QString,    UncheckedToolTip,     QString()) \
    TVND(QString,   QString,    UncheckedStatusTip,   QString()) \

class EIRGUISHARED_EXPORT CheckAction : public QAction
{
    Q_OBJECT
    DECLARE_QPROPERTIES(CHECKACTION_QPROPERTIES);

public:
    explicit CheckAction(QObject * parent=0);
    CheckAction(const QIcon & icon,
                         const QString & text,
                         QObject * parent=0);
    CheckAction(const QString & text,
                         QObject * parent=0);
    void setIcon(const QIcon & icon);
    void setIcon(const QIcon & unchecked,
                 const QIcon & checked);
    void setText(const QString & text);
    void setText(const QString & unchecked,
                 const QString & checked);
    void setIconText(const QString & text);
    void setIconText(const QString & unchecked,
                     const QString & checked);
    void setToolTip(const QString & text);
    void setToolTip(const QString & unchecked,
                    const QString & checked);
    void setStatusTip(const QString & text);
    void setStatusTip(const QString & unchecked,
                      const QString & checked);

signals:

public slots:

private slots:
    void update(bool checked);

private:
    void ctor(void);
};

#endif // CHECKACTION_H
