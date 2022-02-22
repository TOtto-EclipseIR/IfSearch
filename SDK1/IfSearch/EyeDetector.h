#pragma once

#include "DetectorResult.h"
#include "HaarDetector.h"

class EyeDetector : public HaarDetector
{
    Q_OBJECT

public:
    EyeDetector(QObject *parent = 0);
    ~EyeDetector();
    bool process(QList<DetectorResult> * resultList=0, bool returnAll=false);
    virtual int scaleScore(qreal raw) { return HaarDetector::scaleScore(raw); }
    virtual QQRect doAdjust(QQRect r) { return r; }

signals:
    void processed(void);
}; // class EyeDetector
