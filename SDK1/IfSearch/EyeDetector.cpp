#include <EyeDetector.h>

#include <QQRect.h>
#include <InfoMacros.h>

EyeDetector::EyeDetector(QObject * parent)
    : HaarDetector("Eye", 0.125, parent)
{
    setObjectName("EyeDetector");
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

