#pragma once

#include "HaarDetector.h"

class FrontalFaceDetector : public HaarDetector
{
    Q_OBJECT

public:
    FrontalFaceDetector(QObject *parent = 0);
    ~FrontalFaceDetector();
    bool process(QList<DetectorResult> * resultList=0, bool returnAll=false);
    virtual int scaleScore(qreal raw) { return HaarDetector::scaleScore(raw); }
    virtual QQRect doAdjust(QQRect r);

signals:
    void processed(void);
}; // class FrontalFaceDetector

