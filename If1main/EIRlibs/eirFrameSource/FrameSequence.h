#ifndef FRAMESEQUENCE_H
#define FRAMESEQUENCE_H
#include "eirFrameSource.h"

#include <QObject>

class INDImageClient;

class EIRFRAMESOURCESHARED_EXPORT FrameSequence : public QObject
{
    Q_OBJECT
public:
    explicit FrameSequence(INDImageClient * imClient,
                           QObject * parent=0);
    
signals:
    
public slots:

private:
    INDImageClient * _imClient;
};

#endif // FRAMESEQUENCE_H
