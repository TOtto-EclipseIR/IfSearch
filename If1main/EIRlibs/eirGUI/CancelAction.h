#ifndef CANCELACTION_H
#define CANCELACTION_H

#include "CheckAction.h"

class EIRGUISHARED_EXPORT CancelAction : public CheckAction
{
    Q_OBJECT
public:
    explicit CancelAction(QObject * parent=0);
    bool isCancelled(void) const;

signals:
    void cancelled(void);

public slots:
    void cancel(void);
    void reset(void);

private slots:
    void onToggle(bool checked);
};

#endif // CANCELACTION_H
