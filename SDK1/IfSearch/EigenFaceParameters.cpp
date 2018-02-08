#include <EigenFace.h>
#include "EigenFaceParameters.h"

#include <QByteArray>
#include <QDomDocument>
#include <QDomElement>
#include <QString>




Return EigenFaceParameters::staticReturn;

EigenFaceParameters::EigenFaceParameters(void)
{
} // c'tor

EigenFaceParameters::~EigenFaceParameters()
{
} // d'tor

EigenFaceParameters * EigenFaceParameters::create(const QDir & initDir) 
{ 
    (void)initDir;

    staticReturn.clear();

    EigenFaceParameters * ptr = new EigenFaceParameters;
    if ( ! ptr)
    {
        staticReturn = Return(EigenFace::ReturnStatusNull);
        return ptr;
    }

    ptr->minEyeDist = 17; // 34;
    ptr->maxEyeAdj = 1;
    //	TemplateVectors << EigenFaceVectorType::Raw;
    ptr->maxAvgsInTemplate = 0;
    ptr->normalSize = QSize(256, 256);
    ptr->eyeDistance = 64;
    ptr->layersInVector = 320;
    return ptr;
} // create()

Return EigenFaceParameters::init(const QDomElement & de)
{
    (void)de;
    return Return();
}

Eyes EigenFaceParameters::normalEyes(void) const
{
    QPoint ctr(normalSize.width() / 2, normalSize.height() / 2);
    QPoint lEye = ctr + QPoint( - eyeDistance / 2, - eyeDistance / 2);
    QPoint rEye = ctr + QPoint( + eyeDistance / 2, - eyeDistance / 2);
    return Eyes(lEye, rEye);
}
