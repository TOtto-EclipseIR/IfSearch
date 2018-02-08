#include <EigenFaceGenerator.h>

#include <Detector.h>
#include <EigenFace.h>
#include <EigenFaceData.h>
#include <EigenFaceParameters.h>
#include <EigenFaceTemplate.h>

#include <QByteArray>
#include <QPointF>
#include <QString>
#include <QTime>
// IEIGENFACE_PRELIMINARY
//#include <QPainter>
//#include <QPen>

#include <FeatureSet.h>
#include <ImageInfo.h>
#include <InfoMacros.h>
#include <QQRect.h>


using namespace INDI;



DDT::Return EigenFaceGenerator::generateTemplate(int minConsistencyOverride)
{
    DDT::Return rtn;
    QTime localTime;
    localTime.start();
        QTime scaleTime;
        scaleTime.start();
        HeadScale = qRound(5.0 * eigenEyes().distance()
                           / (qreal)(head.isEmpty() ? originalImage.width() : head.width()));
        if (HeadScale < 2.0)
            HeadScale = 1.0;
        else
        {
            if (HeadScale > 5.0)
                HeadScale = 5.0;
            originalImage = originalImage.scaled(originalImage.size() * HeadScale,
                                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            head = QRect(head.topLeft() * HeadScale, head.bottomRight() * HeadScale);
            origEyes = Eyes(origEyes.p1() * HeadScale, origEyes.p2() * HeadScale);
        }
        MsecScale = scaleTime.elapsed();
        // adjust eye position for best residual
        // side effect: gets difference array for mean face zero, raw input
        rtn = adjustEyes();
        if (rtn.isError())
        {
            MsecGenerate = localTime.elapsed();
            return rtn;
        }

        // calculate other difference arrays

        // foreach(differences), calculate eigen vector
        if (differences.contains(EigenFaceVectorKey()))
        {
            EigenFaceMaskedArray diff = differences.value(EigenFaceVectorKey());
            EigenFaceVector vec = diff.generateVector(parms->numLayersInVector());
            fSet.set(DDT::Feature::TemplateMethod, methodString());
            fSet.set(DDT::Feature::Consistency, vec.consistency());
            Consistency = vec.consistency();
            if (vec.consistency() > (minConsistencyOverride
                                     ? minConsistencyOverride
                                     : minConsistency()))
            {
                tpl.insert(vec.residual(), vec);
            }
        }

        if (tpl.isEmpty())
        {
            MsecGenerate = localTime.elapsed();
            return DDT::Return(EigenFace::ReturnLowConsistency);
        }
    MsecGenerate = localTime.elapsed();
    return DDT::Return();
} // generateTemplate()

DDT::Return EigenFaceGenerator::findHead(void)
{
    QList<DetectorResult> resultList;

    // Prerequisites
    if ( ! data || ! data->isValid())
        return DDT::Return(EigenFace::ReturnNoData);
    if ( ! parms || ! parms->isValid())
        return DDT::Return(EigenFace::ReturnNoParameters);
    if ( ! ffd || ! ffd->isValid() || ! ffd->hasDetector())
        return DDT::Return(EigenFace::ReturnNoDetector, "Face");
    if (originalImage.isNull())
        return DDT::Return(EigenFace::ReturnNullImage, "Original Image");


        // Detect Head
        ffd->setImage(originalImage);
        if ( ! ffd->process(&resultList))
            return DDT::Return(EigenFace::ReturnDetectorError, "Face");
        if (resultList.isEmpty())
            head = originalImage.rect();
        else
        {
            head = resultList.at(0).rectangle();
            eyeRoiMethod = Neighbors;
            fSet.set(DDT::Feature::HeadMethod, ffd->methodString());
            fSet.set(DDT::Feature::HeadBox,
                     QRect(head.topLeft() / HeadScale,
                           head.bottomRight() / HeadScale));
            fSet.set(DDT::Feature::Quality, resultList.at(0).score());
        }
    return DDT::Return();
} // findHead()


DDT::Return EigenFaceGenerator::findEyes(void)
{
    DDT::Return rtn;
    QList<DetectorResult> resultList;

    // Prerequisites
    if ( ! data || ! data->isValid())
        return DDT::Return(EigenFace::ReturnNoData);
    if ( ! parms || ! parms->isValid())
        return DDT::Return(EigenFace::ReturnNoParameters);
    if ( ! leyed || ! leyed->isValid() || ! leyed->hasDetector())
        return DDT::Return(EigenFace::ReturnNoDetector, "Left Eye");
    if ( ! reyed || ! reyed->isValid() || ! reyed->hasDetector())
        return DDT::Return(EigenFace::ReturnNoDetector, "Right Eye");
    if (originalImage.isNull())
        return DDT::Return(EigenFace::ReturnNullImage, "originalImage");

    if ( ! head.isValid())
        rtn = findHead();
    if (rtn.isError())
        return rtn;

        // Find Regions of Interest for both eyes
        QPoint leftRoiCenter, rightRoiCenter;
        QSize roiSize;
        switch (eyeRoiMethod)
        {
        default:
            leftRoiCenter  = head.center()
                             + QPoint( - head.width() / 4, - head.height() / 4);
            rightRoiCenter = head.center()
                             + QPoint( + head.width() / 4, - head.height() / 4);
            roiSize = QSize(head.width() / 2, head.height() / 2);
            expectedEyeWidth = head.width() / 8;
            eyeWidthFactor = 2.0;
            break;
        } // eyeRoiMethod
        if (roiScale())
            roiSize *= (qreal)roiScale() / 100.0;
        if (eyeScale())
            eyeWidthFactor *= (qreal)eyeScale() / 100.0;
        if (roiSize.width() < qMin(leyed->detectorSize().width(),
                                   reyed->detectorSize().width()))
            return DDT::Return(EigenFace::ReturnNoEyes);
        DDT::QQRect leftRoi(roiSize, leftRoiCenter);
        DDT::QQRect rightRoi(roiSize, rightRoiCenter);
        ExpectedEyes.set(leftRoiCenter, rightRoiCenter);

        // Setup Eye Detector
        leyed->setMinPixels((qreal)expectedEyeWidth / eyeWidthFactor);
        leyed->setMaxPixels((qreal)expectedEyeWidth * eyeWidthFactor);
        reyed->setMinPixels((qreal)expectedEyeWidth / eyeWidthFactor);
        reyed->setMaxPixels((qreal)expectedEyeWidth * eyeWidthFactor);
        leyed->setFactor(factor());
        reyed->setFactor(factor());

        // Detect Left Eye
        leftRoi &= originalImage.rect();
        leyed->setImage(originalImage.copy(leftRoi));
        LeftEyeRoi = leftRoi;
        if ( ! leyed->process(&resultList))
            return DDT::Return(EigenFace::ReturnDetectorError, "Left Eye");
        LeftEyePerformance = leyed->performanceString();
        if (resultList.isEmpty())
            return DDT::Return(EigenFace::ReturnNoEyes);
        QRect leftRect = resultList.at(0).rectangle();
        QPoint leftEye(leftRect.center());
        leftEye += leftRoi.topLeft();

        // Detect Right Eye
        if (rightRoi.left() < leftEye.x())
            rightRoi.setLeft(leftEye.x());
        rightRoi &= originalImage.rect();
        RightEyeRoi = rightRoi;
        reyed->setImage(originalImage.copy(rightRoi));
        if ( ! reyed->process(&resultList))
            return DDT::Return(EigenFace::ReturnDetectorError, "Right Eye");
        RightEyePerformance = reyed->performanceString();
        if (resultList.isEmpty())
            return DDT::Return(EigenFace::ReturnNoEyes);
        QRect rightRect = resultList.at(0).rectangle();
        QPoint rightEye(rightRect.center());
        rightEye += rightRoi.topLeft();

        if (EigenFace::pitch(leftEye, rightEye) > 0.5)
            return DDT::Return(EigenFace::ReturnNoEyes);

        origEyes = Eyes(leftEye, rightEye);
        fSet.set(DDT::Feature::EyeMethod, leyed->methodString()
                 + "|" + leyed->methodString());
        fSet.set(DDT::Feature::LeftEye, leftEye / HeadScale);
        fSet.set(DDT::Feature::RightEye, rightEye / HeadScale);
        fSet.calculate();
    return DDT::Return();
} // findEyes()


// 1. Find eye positions that produce the least residual when subtracted
//		from the first average face. This will produce the "official"
//		EigenFaceGenerator image and the <mean0,raw> masked array
//		Prerequisites:
//			a. originalImage
//			b. left and right eye locations in imageInfo
//		Parameters:
//			a. max eye adjustments (if 0 use all, if 1 no adjustment)
//			b. eigen face size and eye positions
//			c. eigen face bit mask [and optional weight mask]
//		Output:
//			a. EigenFaceGenerator[raw]
//			b. EigenArray[mean0, raw]

DDT::Return EigenFaceGenerator::adjustEyes(void)
{
    DDT::Return rtn;

    // Prerequisites
    if ( ! data || ! data->isValid())
        return DDT::Return(EigenFace::ReturnNoData);
    if ( ! parms || ! parms->isValid())
        return DDT::Return(EigenFace::ReturnNoParameters);
    if (originalImage.isNull())
        return DDT::Return(EigenFace::ReturnNullImage, "originalImage");

    if (origEyes.isNull())
    {
        rtn = findEyes();
        if (rtn.isFatal())
            FATAL(rtn.toString());
        else if (rtn.isError())
            return rtn;
    }
    if (origEyes.isNull())
        return DDT::Return(EigenFace::ReturnNoEyes);

    int eyeDistance = origEyes.distance();
    if (eyeDistance
        && parms->minEyeDistance()
        && eyeDistance < parms->minEyeDistance())
        return DDT::Return(EigenFace::ReturnNoEyes);

    Eyes bestEyes;
    EigenFaceMaskedArray bestMeanDifference(data);
    //qreal bestResidual = INT_MAX;
    if (true || eyeDistance <= 40 || 1 == parms->maxEyeAdjust())
    {
        // No Adjusting Eye Positions
        QTime localTime;
        localTime.start();
        rtn = bestMeanDifference.generate(originalImage, origEyes);
        MsecTemplate = localTime.elapsed();
        Consistency = bestMeanDifference.consistency();
        if (rtn.isError())
            return rtn;
        bestEyes = origEyes;
        //		errorEmit("Template Generated");
    }
    else
    {
        // TODO
        return DDT::Return(DDT::Return::NotImplemented, "Acutally adjustEyes()");
    }

    if (bestEyes != origEyes)
        clear(ClearAdjusted);
    adjEyes = bestEyes;

    eigenImage = bestMeanDifference.image();
    differences.insert(bestMeanDifference.vectorKey(), bestMeanDifference);
    return DDT::Return();
} // adjustEyes()

DDT::Return EigenFaceGenerator::generateNormalized(void)
{
    if ( ! data)
        return DDT::Return(EigenFace::ReturnNoData);
    Eyes normalEyes = parms->normalEyes();
    if (normalEyes.isNull())
        return DDT::Return(EigenFace::ReturnInvalidStructure, "normalEyes");
    QSize normalSize = parms->normalizedSize();
    if ( ! normalSize.isValid())
        return DDT::Return(EigenFace::ReturnInvalidStructure, "normalSize");
    if (adjEyes.isNull())
        return DDT::Return(EigenFace::ReturnNoEyes);
    TRACE("originalImage=%1,%2 normalSize=%3,%4",
          originalImage.width(), originalImage.height(),
          normalSize.width(), normalSize.height());
    TRACE("adjEyes=%1,%2~%3,%4",
          adjEyes.left().x(), adjEyes.left().y(),
          adjEyes.right().x(), adjEyes.right().y());
    normalImage = EigenFace::normalize(originalImage, adjEyes, normalSize, normalEyes);
    DDT::QQRect normalHead(normalSize);
    normalHead = normalHead.scaled((qreal)data->eigenEyes().width()
                                   / (qreal)normalEyes.width());
    DDT::FeatureSet fSet;
    fSet.set(DDT::Feature::ImageSize, normalImage.size());
    fSet.set(DDT::Feature::HeadBox, (QRect)normalHead);
    fSet.set(DDT::Feature::EyeLine, (QLine)normalEyes);
    fSet.set(DDT::Feature::Consistency, consistency());
    fSet.calculate();
    DDT::ImageInfo ii;
    ii.addFace(fSet);
    ii.setImageText(&normalImage);

    return DDT::Return();
} // generateNormalized()
