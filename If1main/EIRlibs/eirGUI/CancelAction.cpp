#include "CancelAction.h"

#include "../eirExe/Log.h"
#include "QQIcon.h"

CancelAction::CancelAction(QObject *parent)
    : CheckAction(parent)
{
    CheckAction::setIcon(QQIcon("action-cancel"),
                         QQIcon("action-cancelled"));
    CheckAction::setText("Cancel", "Cancelled");
    CONNECT(this, SIGNAL(toggled(bool)),
            this, SLOT(onToggle(bool)));
}

bool CancelAction::isCancelled(void) const
{
    return CheckAction::isChecked();
}

void CancelAction::cancel(void)
{
    if ( ! CheckAction::isChecked())
        CheckAction::toggle();
}

void CancelAction::reset(void)
{
    if (CheckAction::isChecked())
        CheckAction::toggle();
}

void CancelAction::onToggle(bool checked)
{
    if (checked)
        emit cancelled();
}
