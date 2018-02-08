/** @file EigenFace.h
*
*	Common, Public declarations for INDI project EigenFace
*/

#pragma once
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

class QDir;
//class QLine;
class QProgressBar;
class QSize;
#include <QImage>

#include <Eyes.h>

#include <NamedOrdinal.h>
#include <Return.h>
namespace DDT
{
    class GreyImage;
}

namespace INDI
{
    class EigenFaceData;
    class EigenFaceParameters;

    class INDIEF_EXPORT EigenFaceStatus : public DDT::NamedOrdinal
    {
        DECLARE_NAMEDORDIAL_CTORS(EigenFaceStatus)
		static const EigenFaceStatus Inconsistent;
        static const EigenFaceStatus Active;
        static const EigenFaceStatus Deleted;
        static const EigenFaceStatus Error;
    }; // EigenFaceStatus

    class INDIEF_EXPORT EigenFace
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
        static QImage normalize(const DDT::GreyImage & gi,
                                const Eyes inputEyes,
                                const QSize outSize,
                                const Eyes outEyes,
                                QImage::Format newFormat=QImage::Format_Invalid);
        //		static qreal distance(const QLine line);
        static qreal pitch(QPointF p1, QPointF p2);
        //		static EigenFaceData * data(const QDir & initDir);
        static EigenFaceParameters * parameters(const QDir & initDir);
        static DDT::Return lastReturn(void) { return staticReturn; }

    private:
        static DDT::Return staticReturn;
    }; // EigenFace
} // INDI
