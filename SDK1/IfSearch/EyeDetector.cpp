#include <Detector.h>
#include <QQRect.h>

#include <InfoMacros.h>



EyeDetector::EyeDetector(const unsigned interface,
                         QObject * parent) :
    HaarDetector("Eye",
        0.125,
        (interface < HaarDetector::nInterface) ? interface : 0,
        parent)
{
    interface_ = interface_ ? interface_ : HaarDetector::CascadeClassifier;
} // c'tor

EyeDetector::~EyeDetector()
{
} // d'tor

bool EyeDetector::process(QList<DetectorResult> * resultList, bool returnAll)
{
    DETAIL(this->methodString());
    bool rtn = HaarDetector::process(returnAll);
    if (rtn)
    {
        groupObjects(returnAll);
        if (resultList)
            *resultList = results;
        else
            emit processed();
    }
    return rtn;
}

