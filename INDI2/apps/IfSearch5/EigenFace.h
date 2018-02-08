/** @file EigenFace.h
*
*	Common, Public declarations for INDI project EigenFace
*/

#pragma once
#include <qglobal.h>

class QDir;
//class QLine;
class QProgressBar;
class QSize;
#include <QImage>

#include <Eyes.h>
#include <Return.h>
class GreyImage;

class EigenFaceData;
class EigenFaceParameters;

class  EigenFace
{
public:
    enum Returns
    {
        ReturnStatusNull = 0x85030101,
        ReturnNoData, ReturnNoTemplate, ReturnNoParameters, ReturnConflict,
        ReturnNoDetector, ReturnDetectorError, ReturnException,
        ReturnBadFltFile, ReturnInvalidStructure, ReturnNullImage,
        ReturnNoHead, ReturnNoEyes, ReturnLowConsistency, ReturnEmptyTemplate,
        ReturnAlreadyEnrolled, ReturnNotEnrolled, ReturnEnrolled,
    };

    static QImage normalize(const QImage & input,
                            const Eyes inputEyes,
                            const QSize outSize,
                            const Eyes outEyes,
                            QImage::Format newFormat=QImage::Format_Invalid);
    static QImage normalize(const GreyImage & gi,
                            const Eyes inputEyes,
                            const QSize outSize,
                            const Eyes outEyes,
                            QImage::Format newFormat=QImage::Format_Invalid);
    //		static qreal distance(const QLine line);
    static qreal pitch(QPointF p1, QPointF p2);
    //		static EigenFaceData * data(const QDir & initDir);
    static EigenFaceParameters * parameters(const QDir & initDir);
    static Return lastReturn(void) { return staticReturn; }

private:
    static Return staticReturn;
}; // EigenFace
