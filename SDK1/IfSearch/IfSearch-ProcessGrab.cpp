#include "IfSearch.h"

#include <QBitmap>
#include <QPixmap>
#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceSearcher.h>
#include <EigenFaceParameters.h>
#include <EigenFaceTemplate.h>
#include <FaceBase.h>

#include <Eyes.h>
#include <FileWriteProfile.h>
#include <ImageCache.h>
#include <ImageMarker.h>
#include <ImageSource.h>
#include <InfoMacros.h>
#include <InputHotdir.h>
#include <QQRect.h>
#include <Return.h>
#include <Settings.h>

#include <HeightGrid.h>
#include <SkinDetector.h>
#include <SkinMatcher.h>
#include <CharacteristicColor.h>
#include <ClothesMatchProperties.h>
#include <ClothesMatcher.h>



void IfSearch::processGrab(void)
{
    FUNCTION();
    QList<DetectorResult> resultList;

    if (paused
            || ffdBusy
            || imageCache.isGrabEmpty()
            || ! pendingFaces.isEmpty()
            || ! commandMode.isEmpty())
    {
        if (writer)
            writer->pumpAll();
        if (ffdBusy)
            DETAIL("processGrab() FFD is busy");
        if (paused)
            DETAIL("processGrab() paused");
        if (imageCache.isGrabEmpty())
        {
            appSettings->setValue("Input/Processing", "done");
            DETAIL("processGrab() empty grab cache");
        }
        if (pendingFaces.isEmpty())
            DETAIL("processGrab() has no pending faces");
        if ( ! commandMode.isEmpty())
            DETAIL("Busy with %1", commandMode);
        return;
    }

    bool detectEnabled = optDetectEnable->toBool();
    if (detectEnabled && ! ffd->hasDetector())
    {
        WARNING("No Facial Detector Selected");
        detectEnabled = false;
    }

    qreal inputOverCrop = optInputOverCrop->toInt()
                          ? 100.0 / (qreal)optInputOverCrop->toInt() : 0.0;
    QString imageId = imageCache.takeGrab();
    QImage image = imageCache.getImage(imageId);
    QDateTime grabTime = imageCache.timeStamp(imageId);
    if (streamBase_mst.isNull())
        streamBase_mst.set(grabTime);

    idGenerator.reset();
    idGenerator.setFrameFormat(appSettings->value("Output/FrameIdFormat",
                                        "%i").toString());
    idGenerator.setFaceFormat(appSettings->value("Output/FaceIdFormat",
                                        "%i-%x%y%q%w%c%e%m").toString());
    DETAIL("FrameIdFormat=%1", idGenerator.frameFormat());
    DETAIL("FaceIdFormat=%1", idGenerator.faceFormat());
    idGenerator.FrameId = imageId;
    idGenerator.FrameMsd = streamBase_mst.delta(grabTime);
    idGenerator.ModifiedMst = grabTime.toMSecsSinceEpoch();
    idGenerator.FrameNumber = 1+FramesProcessed;
    idGenerator.FileName = imageCache.fileName(imageId);

    QImage imageRgb;
    imageCache.hold(imageId, ffd);
    if (frameStatistics)
        frameStatistics->start(grabTime, imageCache.grabSize(), image.size());

    if (camera)
        imageCache.release(imageId, camera);
    if (hotdir)
        imageCache.release(imageId, hotdir);

    if (image.isNull())
    {
        WARNING("***Null Image: %1", imageId);
        if ( ! imageCache.isGrabEmpty())
            QTimer::singleShot(10, this, SLOT(processGrab()));
        imageCache.release(imageId);
        appSettings->setValue("Output/NullFrames", ++NullFrames);
        return;
    }

    PROGRESS("===Processing %2\t%1 modified %3 [%4]",
             imageId, 1+FramesProcessed,
             grabTime.toString("yyyyMMdd hhmmsszzz"),
             grabTime.toMSecsSinceEpoch());
    appSettings->setValue("Input/Processing", imageId);
    Info::flush();

    int maxDim = appSettings->value("Input/MaxDimension", 1536).toInt();
    if (xformProps.dimension(image.size()) > maxDim)
    {
        INFO("***Image too big: %1 %2,%3",
             imageId, image.width(), image.height());
        qreal xfScale = qIsNull(xformProps.getScale())
                ? 1.0 : xformProps.getScale();
        int dim = qMax(image.width(), image.height());
        int dimDiv = 1 + dim / maxDim;
        TRACE("dimDiv=%1", dimDiv);
        if (xfScale > (1.0 / (qreal)dimDiv))
        {
            TRACE("setScale(%1)", 1.0 / (qreal)dimDiv);
            xformProps.setScale(1.0 / (qreal)dimDiv);
        }
    }

    if ( ! xformProps.isNull())
    {
        INFO("Transforming Image");
        image = image.transformed(xformProps.transform());
    }

    if (detectEnabled)
    {
        ffdBusy = true;
        ffd->setImage(image);
        if ( ! ffd->process(&resultList))
        {
            ERRMSG("Error in Face Detector for %1", imageId);
            resultList.clear();
        }
        else
        {
            if (frameStatistics)
                frameStatistics->detected(resultList.size(),
                                          ffd->raw(),
                                          ffd->strange(),
                                          ffd->msec());
            DETAIL("   %1", ffd->performanceString());
        }
    }
    else
    {
        resultList.clear();
        if ( ! qIsNull(inputOverCrop) && optGenerateEnable->toBool())
        {
            QRect head = QQRect(image.size() * inputOverCrop,
                               image.rect().center());
            DetectorResult result(1, 0, head);
            resultList.append(result);
        }
    }

    if ( ! resultList.isEmpty())
        idGenerator.Quality = resultList.last().score();

    if (fwpCharcol->isActive())
    {
        if (imageRgb.isNull())
            imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QImage charcolImage = CharacteristicColor::process(imageRgb);
        RETURN(fwpCharcol->write(charcolImage, idGenerator.frame("Charcol")));
    }
    if (fwpSkin->isActive())
    {
        if (imageRgb.isNull())
            imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QImage skinImage = skinDetector->masked(imageRgb);
        RETURN(fwpSkin->write(skinImage, idGenerator.frame("Skin")));
    }

    if ( ! resultList.isEmpty())
        idGenerator.BestQuality = (resultList.first().score());
    if (fwpDetect->isActive() && detectEnabled)
        RETURN(fwpDetect->write(ffd->detectImage(), idGenerator.frame("Detect")))

    if (resultList.isEmpty())
    {
        ffd->clear();
        ffdBusy = false;
        if (pausePending)
        {
            pausePending = false;
            paused = true;
            imageCache.flushGrab();
            pendingFaces.clear();
            PROGRESS("---PAUSED---");
        }
        INFO("^^^No faces in %1", imageId);
        RETURN(fwpNoFace->write(image, idGenerator.frame("NoFace")));
        RETURN(fwpCapture->write(imageId));
        RETURN(fwpCapture2->write(image, idGenerator.frame("Input")));
        if (fpWriter && framePerformance && frameStatistics)
        {
            frameStatistics->finish(0, imageCache.status());
            framePerformance->calculate(frameStatistics, imageId);
            fpWriter->writeLine();
        }

        if (fwpHeight->isActive() && optForceHeight->toBool())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            NULLPTR(heightGrid);
            heightGrid->clear();
            QImage heightImage(heightGrid->gridImage(imageRgb));
            fwpHeight->write(heightImage, idGenerator.face("Height"));
        }

        if (optForceMarked->toBool() && fwpMarked->isActive())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            markedImage = imageRgb;
            ImageMarker marker(&markedImage);

            // background image?
            QString bgFileName = optMarkBackgroundFile->toString();
            if ( ! bgFileName.isEmpty())
            {
                QImage bgImage(bgFileName);
                bgImage = bgImage.scaled(markedImage.size(), Qt::KeepAspectRatioByExpanding);
                marker.drawImage(QPoint(0,0), bgImage);
            }

            // background color?
            QColor bgColor = optMarkBackgroundColor->value<QColor>();
            if (bgColor.isValid())
            {
                bgColor.setAlpha(255.0 * optMarkBackgroundTransparency->toDouble() / 100.0);
                marker.fillRect(markedImage.rect(), bgColor);
            }

            marker.end();
            RETURN(fwpMarked->write(markedImage, idGenerator.frame("Marked")));
        }
        appSettings->setValue("Output/FramesProcessed",
                              QString::number(++FramesProcessed));
    }
    else
    {
        imageCache.hold(imageId, eigenFace);

        // --- Start Marked Image if markedDir is active
        isHeight = false;
        if (fwpMarked->isActive())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            markedImage = imageRgb;
            ImageMarker marker(&markedImage);
            isMarked = false;

            // background image?
            QString bgFileName = optMarkBackgroundFile->toString();
            if ( ! bgFileName.isEmpty())
            {
                QImage bgImage(bgFileName);
                bgImage = bgImage.scaled(markedImage.size(), Qt::KeepAspectRatioByExpanding);
                marker.drawImage(QPoint(0,0), bgImage);
            }

            // background color?
            QColor bgColor = optMarkBackgroundColor->value<QColor>();
            if (bgColor.isValid())
            {
                bgColor.setAlpha(255.0 * optMarkBackgroundTransparency->toDouble() / 100.0);
                marker.fillRect(markedImage.rect(), bgColor);
            }
        }

        // setup each face for processing
        //  (starting with the lowest quality so high quality
        //   faces are last marked and therefor on top)
        while ( ! resultList.isEmpty())
        {
            DetectorResult result = resultList.takeLast();
            pendingFaces.append(QPair<QString,DetectorResult>(imageId, result));
            DETAIL("Queueing Face %1 Quality=%2", imageId, result.score());
            if (pendingFaces.isEmpty())
                QTimer::singleShot(10, this, SLOT(processFace()));
        }
        // append a blank face as signal to write marked image
        pendingFaces.append(QPair<QString,DetectorResult>(imageId, DetectorResult()));
    }

    grabImage = image;
    imageCache.release(imageId, ffd);

    if ( ! imageCache.isGrabEmpty())
        QTimer::singleShot(10, this, SLOT(processGrab()));
} // processGrab()
