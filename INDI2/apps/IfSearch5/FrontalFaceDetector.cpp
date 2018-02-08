#include <Detector.h>
#include <QQRect.h>



FrontalFaceDetector::FrontalFaceDetector(QObject * parent) :
    HaarDetector("FaceFrontal", 1.0 / 3.0, parent)
{

} // c'tor

FrontalFaceDetector::~FrontalFaceDetector()
{
} // d'tor

bool FrontalFaceDetector::process(QList<DetectorResult> * resultList, bool returnAll)
{
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

QQRect FrontalFaceDetector::doAdjust(QQRect r)
{
    QSize sz;
    QPoint ctr;

    switch (Adjust)
    {
    case 1:		// down size by 1/4 and move down by 1/8 of height
        // to move top to above brow and suck into ears
        sz = r.size() * 3 / 4;
        ctr = r.center() + QPoint(0, r.height() / 8);
        return QQRect(sz, ctr);

    case 2:		// down size by 1/4, but no movement
        // to correspond with Adj=1 on generic cascades
        // using new INDI cascades
        sz = r.size() * 3 / 4;
        ctr = r.center();
        return QQRect(sz, ctr);

    default:
        return r;
    } // switch
} // doAdjust()

