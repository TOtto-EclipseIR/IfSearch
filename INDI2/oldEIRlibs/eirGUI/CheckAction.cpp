#include "CheckAction.h"

#include "../eirExe/Log.h"

CheckAction::CheckAction(QObject * parent)
    : QAction(parent)
{
    ctor();
}

CheckAction::CheckAction(const QString & text,
                     QObject * parent)
    : QAction(text, parent)
{
    ctor();
    setCheckedText(text);
    setUncheckedText(text);
}

CheckAction::CheckAction(const QIcon & icon,
                     const QString & text,
                     QObject * parent)
    : QAction(icon, text, parent)
{
    ctor();
    setCheckedIcon(icon);
    setUncheckedIcon(icon);
    setCheckedText(text);
    setUncheckedText(text);
}

void CheckAction::ctor(void)
{
    DEFINE_QPROPERTIES_CTORS(CHECKACTION_QPROPERTIES);
    QAction::setCheckable(true);
    QAction::setChecked(false);
    CONNECT(this, SIGNAL(toggled(bool)), this, SLOT(update(bool)));
}

void CheckAction::update(bool checked)
{
    if (checked)
    {
        QAction::setIcon(getCheckedIcon());
        QAction::setText(getCheckedText());
        QAction::setIconText(getCheckedIconText());
        QAction::setToolTip(getCheckedToolTip());
        QAction::setStatusTip(getCheckedStatusTip());
    }
    else
    {
        QAction::setIcon(getUncheckedIcon());
        QAction::setText(getUncheckedText());
        QAction::setIconText(getUncheckedIconText());
        QAction::setToolTip(getUncheckedToolTip());
        QAction::setStatusTip(getUncheckedStatusTip());
    }
}

void CheckAction::setIcon(const QIcon & icon)
{
    setCheckedIcon(icon);
    setUncheckedIcon(icon);
    update(isChecked());
}

void CheckAction::setIcon(const QIcon & unchecked,
                          const QIcon & checked)
{
    setCheckedIcon(checked);
    setUncheckedIcon(unchecked);
    update(isChecked());
}

void CheckAction::setText(const QString & text)
{
    setCheckedText(text);
    setUncheckedText(text);
    update(isChecked());
}

void CheckAction::setText(const QString & unchecked,
                          const QString & checked)
{
    setCheckedText(checked);
    setUncheckedText(unchecked);
    update(isChecked());
}

void CheckAction::setIconText(const QString & text)
{
    setCheckedIconText(text);
    setUncheckedIconText(text);
    update(isChecked());
}

void CheckAction::setIconText(const QString & unchecked,
                              const QString & checked)
{
    setCheckedIconText(checked);
    setUncheckedIconText(unchecked);
    update(isChecked());
}

void CheckAction::setToolTip(const QString & text)
{
    setCheckedToolTip(text);
    setUncheckedToolTip(text);
    update(isChecked());
}

void CheckAction::setToolTip(const QString & unchecked,
                             const QString & checked)
{
    setCheckedToolTip(checked);
    setUncheckedToolTip(unchecked);
    update(isChecked());
}

void CheckAction::setStatusTip(const QString & text)
{
    setCheckedStatusTip(text);
    setUncheckedStatusTip(text);
    update(isChecked());
}

void CheckAction::setStatusTip(const QString & unchecked,
                               const QString & checked)
{
    setCheckedStatusTip(checked);
    setUncheckedStatusTip(unchecked);
    update(isChecked());
}
