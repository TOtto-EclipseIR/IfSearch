#ifndef FRAMESOURCEDIRECT_H
#define FRAMESOURCEDIRECT_H
#include "eirFrameSource.h"

#include "FrameSourceInterfaceBase.h"

class QTimer;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceDirect
        : public FrameSourceInterfaceBase
{
    Q_OBJECT
public:
    explicit FrameSourceDirect(FrameSourcePlugin * parent=0);
    QList<int> frameKeys(const int n=0) const;
    int takeKey(void);
    QList<int> takeKeys(const int n=0);
    
signals:
    void grabbed(int frameKey);
    void complete(ResultSet rset);
    
public slots:
    void configure(const VariableSet & config);
    void start(const int channel=-1,
               const QString &begin=QString(),
               const QString &end=QString());

private slots:
    void pluginStarted(ResultSet rset);
    void pluginGrabbed(ResultSet rset);
    void pluginComplete(ResultSet rset);
    void sample(void);

private:
    QTimer * sample_timer;
    MillisecondTime sample_mst;
    QList<int> frameKey_list;
};

#endif // FRAMESOURCEDIRECT_H
