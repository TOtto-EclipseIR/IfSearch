#ifndef TIMEOUTBAR_H
#define TIMEOUTBAR_H
#include "eirGUI.h"

#include <QtGui/QProgressBar>

#include "../eirTypes/MillisecondTime.h"

class QTimer;

class EIRGUISHARED_EXPORT TimeoutBar : public QProgressBar
{
    Q_OBJECT
    Q_PROPERTY(int update_msec READ updateMsec WRITE setUpdateMsec)
    Q_PROPERTY(bool autoHide_b READ autoHide WRITE setAutoHide)

public:
    explicit TimeoutBar(QWidget * parent=0);
    int updateMsec(void) const { return update_msec; }
    void setUpdateMsec(int v) { update_msec = v; }
    bool autoHide(void) const { return autoHide_b; }
    void setAutoHide(bool v) { autoHide_b = v; }

signals:
    void started(void);
    void updated(void);
    void stopped(void);
    void timeout(void);
    void cleared(void);

public slots:
    void start(int timeoutMsec);
    void stop(void);
    void clear(void);

private slots:
    void update(void);

private:
    int update_msec;
    bool autoHide_b;
    QTimer * _timer;
    MillisecondTime timeout_mst;
};

#endif // TIMEOUTBAR_H
