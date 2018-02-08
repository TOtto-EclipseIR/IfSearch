#ifndef VIGILLIVEFEED_H
#define VIGILLIVEFEED_H
#include "ImVigil.h"

#include "VigilDvr.h"

class IMVIGILSHARED_EXPORT VigilLiveFeed : public QObject
{
    Q_OBJECT
public:
    explicit VigilLiveFeed(VigilDvr * parent=0);
    bool isError(void) const;
    QString errorString(void) const;

signals:
    void started(void);

public slots:
    void start(int channelIndex);

private:
    VigilDvr * _dvr;
    VigilSDK::_CVigilLiveFeed * _live;
    QWidget * _widget;
    QString error_s;
};

#endif // VIGILLIVEFEED_H
