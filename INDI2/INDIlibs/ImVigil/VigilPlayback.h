#ifndef VIGILPLAYBACK_H
#define VIGILPLAYBACK_H
#include "ImVigil.h"

#include <QtCore/QDir>

#include "../../EIRlibs/eirCore/IdGenerator.h"
#include "../../EIRlibs/eirTypes/MillisecondDelta.h"
#include "../../EIRlibs/eirTypes/MillisecondTime.h"
#include "VigilDvr.h"



class IMVIGILSHARED_EXPORT VigilPlayback : public QObject
{
    Q_OBJECT
public:
    explicit VigilPlayback(VigilDvr * parent=0);
    bool isError(void) const;
    QString errorString(void) const;
    void setOutput(const QDir & dir,
                   const QString & frameIdFormat=QString("D%d-T%t"),
                   const int quality=90);

signals:
    void started(void);
    void grabbed(void);
    void finished(void);
    
public slots:
    void start(const int channelIndex,
               const MillisecondTime start,
               const MillisecondTime end,
               const MillisecondDelta sample);
    void grab(void);

private:
    VigilDvr * _dvr;
    VigilSDK::_CVigilPlayback * _play;
    QWidget * _widget;
    IdGenerator frame_idGenerator;
    QString error_s;
    QDir output_dir;
    QString frameIdFormat_s;
    int quality_i;
    //MillisecondTime cue_mst;
    MillisecondTime start_mst;
    MillisecondTime end_mst;
    MillisecondDelta sample_msd;
    int frame_n;
};

#endif // VIGILPLAYBACK_H
