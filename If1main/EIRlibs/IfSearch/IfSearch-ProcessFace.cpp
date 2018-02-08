#include "IfSearch.h"

#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceSearcher.h>
#include <EigenFaceSearchTier.h>
#include <EigenFaceSearchPerson.h>
#include <EigenFaceSearchResultList.h>
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

#include <Rectangle.h>
#include <Resolver.h>
#include <AnyColor.h>
#include <SkinMatcher.h>
#include <HeightGrid.h>
#include <CharacteristicColor.h>
#include <ClothesMatchProperties.h>
#include <ClothesMatcher.h>

#include "../../INDI2/EIRlibs/eirTypes/MillisecondDelta.h"

void IfSearch::processFace(void)
{
    NULLPTR(resolver);

    if (pendingFaces.isEmpty() || ! eigenFace)
        return;

    DDT::Return rtn;
    bool writeFaceInfo = optWriteFaceInfo->toBool();
    QPair<QString,INDI::DetectorResult> face = pendingFaces.takeFirst();
    QString imageId = face.first;
    INDI::DetectorResult result = face.second;
    QRect head = result.rectangle();
    Eyes eyeLine;

    if (head.isNull()) // signal we are done with this frame
    {
        if ((isMarked || optForceMarked->toBool()) && ! markedImage.isNull())
        {
            RETURN(fwpMarked->write(markedImage, idGenerator.frame("Marked")));
            RETURN(fwpUnMarked->write(grabImage, idGenerator.frame("UnMarked")));
        }
        else
            RETURN(fwpNoMark->write(grabImage, idGenerator.frame("NoMark")));
        markedImage = QImage();
        isMarked = false;
        RETURN(fwpCapture->write(imageId));
        RETURN(fwpCapture2->write(grabImage, idGenerator.frame("Input")));
        ffd->clear();
        ffdBusy = false;
        imageCache.doDeferred(imageId);
        imageCache.release(imageId, eigenFace); // TODO: should have a process frame holder
        int fwPending = writer->size();
        if (fwPending)
            DETAIL("%1 files to be written", fwPending);
        writer->pumpAll();
        INFO("^^^Processing complete: %1", imageId);
        if (fpWriter && framePerformance && frameStatistics)
        {
            frameStatistics->finish(fwPending, imageCache.status());
            framePerformance->calculate(frameStatistics, imageId);
            fpWriter->writeLine();
        }

        if (pausePending)
        {
            pausePending = false;
            paused = true;
            PROGRESS("---PAUSED---");
            imageCache.flushGrab();
            pendingFaces.clear();
        }

        appSettings->setValue("Output/FramesProcessed",
                              QString::number(++FramesProcessed));
        DDT::Info::flush();
        return;
    }

    idGenerator.setDetectRect(head);
    idGenerator.setQuality(result.score());
    idGenerator.setFaceNumber(1+FacesProcessed);
    idGenerator.setEyeLine(QLineF());
    idGenerator.setConsistency(0);
    idGenerator.setRank(0);
    idGenerator.setConfidence(0);
    resolver->clearScores();
    resolver->setScore("Quality", result.score());
    int overCrop = optInputOverCrop->toInt();
    qreal cropScale = (overCrop >= 50 && overCrop <= 400)
                      ? ((qreal)overCrop / 100.0) : 2.0;
    DDT::QQRect crop(head.size() * cropScale, head.center());
    QImage cropImage = grabImage.copy(crop);
    QRect cropHead(QRect(head.topLeft() - crop.topLeft(), head.size()));
    PROGRESS("---%1-pixel face at %3, %4 Q%2", head.width(), result.score(),
             head.center().x(), head.center().y());
    QImage grabRgb = grabImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    if (optGenerateEnable->toBool())
    {
        rtn = eigenFace->setImage(cropImage);
        RETURN(rtn);
        eigenFace->setHeadBox(cropHead);
        rtn = eigenFace->generateTemplate();
        if (eigenFace->headScale() > 1.0)
            DETAIL("   Scale:     %1 (x%2)", eigenFace->msecScale(),
                   eigenFace->headScale());
        if ( ! eigenFace->leftEyePerformance().isEmpty())
            DETAIL("   Left Eye:  %1", eigenFace->leftEyePerformance());
        if ( ! eigenFace->rightEyePerformance().isEmpty())
            DETAIL("   Right Eye: %1", eigenFace->rightEyePerformance());
        if (eigenFace->msecTemplate())
            DETAIL("   Template:  %1", eigenFace->msecTemplate());
        if (eigenFace->msecGenerate())
            DETAIL("   Overall:   %1", eigenFace->msecGenerate());
        resolver->setScore("Consistency", eigenFace->consistency());
        eyeLine = eigenFace->adjustedEyes();
        idGenerator.setConsistency(eigenFace->consistency());
        idGenerator.setEyeLine(eyeLine);
    }
    else
    {
        resolver->ignore("Consistency");
        eigenFace->clear();
        if (false) // optForceExpected
            eyeLine = Eyes(head.center() + QPoint( - head.width() / 4,
                                                   - head.height() / 4),
                           head.center() + QPoint( + head.width() / 4,
                                                   - head.height() / 4));
        rtn = DDT::Return();
    }

    // Process results
    QDomDocument docTemplate("INDIface");
    if (writeFaceInfo)
        eigenFace->templat().write(&docTemplate);
    int consistency = eigenFace->consistency();
    if (fwpGenerate->isActive() && optGenerateEnable->toBool())
    {
        qreal headScale = eigenFace->headScale();
        Rectangle headBox = eigenFace->headBox();
        headBox.scale(headScale);
        QImage generateImage = eigenFace->image().convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QColor eyeColor = optMarkEyeColor->value<QColor>();
        QColor roiColor = optMarkEyeRoiColor->value<QColor>();
        QColor allColor = optMarkAllEyeColor->value<QColor>();
        if (roiColor.isValid() || allColor.isValid())
        {
            Eyes expectedEyes = eigenFace->expectedEyes() * headScale;
            Eyes eyeCenters = eyeLine.isNull()
                              ? expectedEyes
                                  : eyeLine * headScale;
            Rectangle leftRoi = eigenFace->leftEyeRoi();
            Rectangle rightRoi = eigenFace->rightEyeRoi();
            QSize expectedEyeSize = eigenFace->expectedEyeSize();
            int minExpectedEyeWidth = eigenFace->miniumumEyeWidth();
            int maxExpectedEyeWidth = eigenFace->maximumEyeWidth();
            DDT::ImageMarker marker(&generateImage);
            marker.rect(headBox, roiColor, 3);
            marker.eyes(expectedEyes, roiColor, 0, 4);
            marker.rect(DDT::QQRect(expectedEyeSize,
                                    eyeCenters.left()),
                        roiColor);
            marker.rect(DDT::QQRect(expectedEyeSize,
                                    eyeCenters.right()),
                        roiColor);
            marker.rect(DDT::QQRect(QSize(minExpectedEyeWidth,
                                          maxExpectedEyeWidth),
                                    expectedEyes.left()),
                        roiColor);
            marker.rect(DDT::QQRect(QSize(minExpectedEyeWidth,
                                          maxExpectedEyeWidth),
                                    expectedEyes.right()),
                        roiColor);
            roiColor.setAlpha(128);
            marker.fillRect(leftRoi.scaled(headScale),
                            QBrush(roiColor, Qt::FDiagPattern));
            marker.fillRect(rightRoi.scaled(headScale),
                            QBrush(roiColor, Qt::BDiagPattern));
        }
        if (eyeColor.isValid())
        {
            DDT::ImageMarker marker(&generateImage);
            marker.eyes(eyeLine * headScale, eyeColor, 2, 5);
        }
        RETURN(fwpGenerate->write(generateImage, idGenerator.face("Generate")));
    }

    if (rtn.is(INDI::EigenFace::ReturnNoHead))
    {
        // we specified head box, so shouldn't be here
        RETURN(rtn);
    }
    else if (rtn.is(INDI::EigenFace::ReturnNoEyes))
    {
        INFO("   No eyes found");
        RETURN(fwpNoEyes->write(cropImage, idGenerator.face("NoEyes")));
        if ( ! markedImage.isNull())
        {
            QColor noEyesColor = optMarkNoEyesColor->value<QColor>();
//            QColor roiColor = optMarkEyeRoiColor->value<QColor>();
            if (noEyesColor.isValid())
            {
                noEyesColor = noEyesColor.lighter(50 + result.score() / 10);
                DDT::ImageMarker marker(&markedImage);
                DDT::QQRect overRect(head);
                DDT::QQRect overMark(head);
                overRect = overRect.scaled(optMarkOverCrop->toDouble() / 100.0);
                overMark = overMark.scaled(optMarkOverMark->toDouble() / 100.0);
                overRect &= markedImage.rect();
                QImage headImage = grabRgb.copy(overRect);
                EXPECT(true,  ! headImage.isNull());
                EXPECT(true, markedImage.rect().contains(overRect));
                marker.drawImage(overRect, headImage);
                marker.rect(overMark, noEyesColor, 3);
                isMarked = true;
            }
        }
    }
    else if (rtn.is(INDI::EigenFace::ReturnLowConsistency))
    {
        INFO("   Face not suitable for searching (Consistency=%1)",
             consistency);
        RETURN(fwpBadFace->write(eigenFace->getNormalImage(), idGenerator.face("BadFace"), docTemplate));
        if ( ! markedImage.isNull())
        {
            QColor badColor = optMarkBadFaceColor->value<QColor>();
            QColor roiColor = optMarkEyeRoiColor->value<QColor>();
            if (badColor.isValid())
            {
                badColor = badColor.lighter(50 + result.score() / 10);
                DDT::ImageMarker marker(&markedImage);
                DDT::QQRect overRect(head);
                DDT::QQRect overMark(head);
                overRect = overRect.scaled(optMarkOverCrop->toDouble() / 100.0);
                overMark = overMark.scaled(optMarkOverMark->toDouble() / 100.0);
                overRect &= markedImage.rect();
                QImage headImage = grabRgb.copy(overRect).convertToFormat(QImage::Format_ARGB32_Premultiplied);
                EXPECT(true,  ! headImage.isNull());
                EXPECT(true, markedImage.rect().contains(overRect));
                marker.drawImage(overRect, headImage);
                marker.rect(overMark, badColor, 3);
                if (optMarkEyeColor->value<QColor>().isValid())
                    marker.eyes(eyeLine.translated(crop.topLeft()), badColor, 3);
                isMarked = true;
            }
        }
    }
    else if (rtn.isError())
    {
        RETURN(rtn);
    }
    else
    {
        QImage normImage;
        Eyes normalEyes;
        if (optGenerateEnable->toBool())
        {
            normImage = eigenFace->getNormalImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
            normalEyes = eigenParms->normalEyes();
            if (optMarkEyes->toBool())
            {
                DDT::ImageMarker marker(&normImage);
                marker.eyes(normalEyes);
                marker.end();
            }
        }
        else
        {
            normImage = cropImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            normalEyes = Eyes::expected(cropImage.size());
        }

        INFO("   Good Consistency %1", consistency);

        AnyColor faceSkinColor;
        QString faceColorMatch;
        int faceColorConfidence;
        if (optFaceColorEnable->toBool())
        {
            skinMatcher->clear();
            skinMatcher->addNormalFaceImage(normImage, normalEyes);
            faceSkinColor = skinMatcher->skinColor();
            faceColorMatch = skinMatcher->bestMatch();
            faceColorConfidence = skinMatcher->bestConfidence();
            if ( ! faceColorMatch.isEmpty())
            {
                if (fwpsFaceColor.value(faceColorMatch+"-Marked")->isActive())
                {
                    QImage faceColorImage = grabRgb;
                    DDT::ImageMarker marker(&faceColorImage);
                    marker.rect(head, optMarkFaceColor->value<QColor>(), 3);
                    marker.end();
                    RETURN(fwpsFaceColor.value(faceColorMatch+"-Marked")->write(faceColorImage, idGenerator.face("FaceColorMarked")));
                }
                if (fwpsFaceColor.value(faceColorMatch+"-Output")->isActive())
                {
                    QImage faceColorImage = normImage;
                    DDT::ImageMarker marker(&faceColorImage);
                    if ( ! faceSkinColor.isNull())
                    {
                        QString colorString = faceSkinColor.string()
                                    + tr("<C%1>%3=%2")
                                        .arg(faceColorConfidence, 3)
                                        .arg(skinMatcher->targetColor(faceColorMatch).string())
                                        .arg(faceColorMatch);
                        marker.title(colorString, Qt::black, faceSkinColor.toQRgb());
                    }
                    marker.rects(skinMatcher->normalFaceRectagles(normalEyes), Qt::blue);
                    RETURN(fwpsFaceColor.value(faceColorMatch+"-Output")->write(faceColorImage, idGenerator.face("FaceColor")));
                }
                DETAIL("FaceSkinColor=%1, Confidence to %2=%3",
                       faceSkinColor.string(), faceColorMatch,
                       faceColorConfidence);
            }
            else
            {
                if (fwpNoFaceColor->isActive())
                {
                    QImage faceColorImage = normImage;
                    DDT::ImageMarker marker(&faceColorImage);
                    if ( ! faceSkinColor.isNull())
                    {
                        QString colorString = faceSkinColor.string();
                        marker.title(colorString, Qt::black, faceSkinColor.toQRgb());
                    }
                    marker.rects(skinMatcher->normalFaceRectagles(normalEyes), Qt::blue);
                    RETURN(fwpNoFaceColor->write(faceColorImage, idGenerator.face("NoFaceColor")));
                }
                DETAIL("FaceSkinColor=%1 No Match", faceSkinColor.string());
            }
            resolver->setScore("FaceColor", faceColorConfidence);
        }
        else
        {
            resolver->ignore("FaceColor");
        }

        if (fwpMarkedFace->isActive())
        {
            if ( ! optMarkEyes->toBool())
            {
                DDT::ImageMarker marker(&normImage);
                marker.eyes(normalEyes);
                marker.end();
            }
            RETURN(fwpMarkedFace->write(normImage, idGenerator.face("MarkedFace"), docTemplate));
        }

        if (fwpRecon->isActive())
        {
            QImage reconImage = eigenFace->getReconImage();
            if ( ! reconImage.isNull())
                RETURN(fwpRecon->write(reconImage, idGenerator.face("Recon") +"-Recon0"));
        }

        if (fwpVector->isActive())
        {
            QImage vectorImage = eigenFace->graphVector();
            if ( ! vectorImage.isNull())
                RETURN(fwpVector->write(vectorImage, idGenerator.face("Vector") +"-Vector0"));
        }

        QRect bodyRect;
        QColor bodyColor = appSettings->value("Output/MarkBodyColor").value<QColor>();
        if (fwpBody->isActive() || bodyColor.isValid())
        {
            qreal bhs = appSettings->value("Mark/BodyHeightScale", 500).toDouble() / 100.0;
            qreal bws = appSettings->value("Mark/BodyWidthScale", 200).toDouble() / 100.0;
            qreal bho = appSettings->value("Mark/BodyHeightOffset", -100).toDouble() / 100.0;
            bodyRect.setTop(head.top() - bho * (qreal)head.height());
            bodyRect.setHeight(bhs * (qreal)head.height());
            bodyRect.setLeft(head.center().x() - 0.5 * bws * (qreal)head.width());
            bodyRect.setWidth(bws * (qreal)head.width());
            QImage bodyImage = grabImage.copy(bodyRect & grabImage.rect());
            fwpBody->write(bodyImage, idGenerator.face("Body"));
        }

        if (optClothesEnable->toBool())
        {
            clothesMatcher->setFace(grabImage, eyeLine.translated(crop.topLeft()));
            //          clothesMatcher->setSkinColor(Color(RGB, faceSkinColor));
            clothesMatcher->calculate();
            if (clothesMatcher->isMatch() || fwpNoClothes->isActive())
            {
                QImage clothesImage = grabRgb;
                DDT::ImageMarker marker(&clothesImage);
                marker.rect(head, optMarkFaceColor->value<QColor>(), 3);
                marker.end();
                if (clothesMatcher->isMatch())
                    fwpClothes->write(clothesImage, idGenerator.face("Clothes"));
                else
                    fwpNoClothes->write(clothesImage, idGenerator.face("NoClothes"));
            }
            if (fwpClothesFace->isActive() && clothesMatcher->isMatch())
                fwpClothesFace->write(normImage, idGenerator.face("ClothesFace"));
            INFO("Upper: Color = %1 %3 Confidence = %2",
                 clothesMatcher->upperColorNames(),
                 clothesMatcher->upperConfidence(),
                 clothesMatcher->upperColorString());
            INFO("Lower: Color = %1 %3 Confidence = %2",
                 clothesMatcher->lowerColorNames(),
                 clothesMatcher->lowerConfidence(),
                 clothesMatcher->lowerColorString());
            INFO("Clothes %1 match", clothesMatcher->isMatch() ? "do" : "DON'T");
            resolver->setScore("UpperClothes", clothesMatcher->upperConfidence());
            resolver->setScore("LowerClothes", clothesMatcher->lowerConfidence());
        }
        else
        {
            resolver->ignore("UpperClothes");
            resolver->ignore("LowerClothes");
        }

        if (optHeightEnable->toBool())
        {
            NULLPTR(heightGrid);
            heightGrid->clear();
            heightGrid->set(eyeLine.translated(crop.topLeft()),
                            grabImage.size(), consistency);
            INFO("Height: Estimated=%1 Confidence=%2",
                 heightGrid->heightEstimate(), heightGrid->confidence());
            if (fwpNoHeight->isActive() || fwpHeight->isActive())
            {
                QImage heightImage(heightGrid->gridImage(grabRgb));
                if (fwpHeight->isActive() && heightGrid->isMatch())
                    fwpHeight->write(heightImage, idGenerator.face("Height"));
                else if (fwpNoHeight->isActive() && ! heightGrid->isMatch())
                    fwpNoHeight->write(heightImage, idGenerator.face("NoHeight"));
            }
            if (fwpHeightFace->isActive() && heightGrid->isMatch())
                fwpHeightFace->write(normImage, idGenerator.face("HeightFace"));
            resolver->setScore("Height", heightGrid->confidence());
        }
        else
        {
            resolver->ignore("Height");
        }

#ifdef ENABLE_AVGFACE
        if (optAvgFaceEnable->toBool()
                && consistency > optAvgFaceMinConsistency->toInt())
            avgFaceProcess(normImage, normalEyes);
#endif

        EigenFaceSearchPerson bestResult;
        if (optMatchEnable->toBool() && ! eigenFace->templat().isEmpty())
        {
            MillisecondTime searchMst = MillisecondTime::current();
            EigenFaceSearchResultList resList
                    = eigenMatcher->search(INDI::EigenFaceSearchSettings::CasualMatch,
                                           eigenFace->templat(),
                                           &matchSettings);
            ++numSearches;
            msecSearches += searchMst.delta();
            idGenerator.setConfidence(eigenMatcher->bestConfidence());
            INFO("   Search: %1 msec against %2 faces, average %3",
                 (qint64)searchMst.delta(),
                 eigenMatcher->size(),
                 msecSearches / numSearches);
            if (resList.isEmpty())
            {
                bestResult.setTier(EigenFaceSearchTier::NoMatch);
                INFO("   No Results - Best Confidence = %1 (%2)",
                     eigenMatcher->bestConfidence(),
                     eigenMatcher->leastDistance());
                RETURN(fwpNoMatch->write(normImage, idGenerator.face("NoMatch"), docTemplate));
                RETURN(fwpFaceCache->write(normImage, idGenerator.face("NoMatch"), docTemplate));
            }
            else
            {
                bestResult = resList.at(0);
                INFO("   Best of %1 Results: %2 %3:%4", resList.size(),
                     bestResult.getDistance(), bestResult.getPersonKey(),
                     bestResult.bestFaceKey());
                if (fwpImage->isActive())
                    writeOutputImage(face, consistency, normImage, resList);
                if (fwpMatch->isActive())
                    writeMatches(resList);
                if (fwpXml->isActive())
                    writeXmlResult(face, resList);
            } // else ResList empty
        } // MatchEnabled
        else
        {
            bestResult.setTier(EigenFaceSearchTier::NoMatch);
            idGenerator.setRank(0);
            idGenerator.setConfidence(0);
            idGenerator.setTier(EigenFaceSearchTier::NoMatch);
            QString faceId = idGenerator.face("Face");
            RETURN(fwpFace->write(normImage, faceId, docTemplate));
            RETURN(fwpFaceCache->write(normImage, faceId, docTemplate));
        }

        if ( ! bestResult.isEmpty())
        {
            idGenerator.setRank(1);
            idGenerator.setConfidence(bestResult.getConfidence());
            idGenerator.setTier(bestResult.getTier().indicator());
            QString matchId = idGenerator.face("Match");
            if (optAppendPersonId->toBool())
                matchId += QString("=M%1%3-%2")
                                .arg(bestResult.getConfidence(), 3)
                                .arg(bestResult.getPersonKey()
                                     ? faceBase->personId(bestResult.getPersonKey())
                                     : faceBase->faceId(bestResult.bestFaceKey()))
                                .arg(bestResult.getTier().indicator());
            RETURN(fwpFace->write(normImage, matchId, docTemplate));
            RETURN(fwpFaceCache->write(normImage, matchId, docTemplate));
        }

        if ( ! markedImage.isNull())
        {
            DDT::ImageMarker marker(&markedImage);
            QColor faceColor = bestResult.getTier().color();
            QColor eyeColor = optMarkEyeColor->value<QColor>();
            if (faceColor.isValid())
            {
                QColor markColor = faceColor.lighter(50 + result.score() / 10);
                if (markColor.isValid())
                {
                    DDT::QQRect overRect(head);
                    DDT::QQRect overMark(head);
                    overRect = overRect.scaled(optMarkOverCrop->toDouble() / 100.0);
                    overMark = overMark.scaled(optMarkOverMark->toDouble() / 100.0);
                    overRect &= markedImage.rect();
                    QImage headImage = grabRgb.copy(overRect);
                    EXPECT(true,  ! headImage.isNull());
                    EXPECT(true, markedImage.rect().contains(overRect));
                    INFO("overRect: %3@%1,%2", overRect.center().x(),
                         overRect.center().y(), overRect.width());
                    marker.drawImage(overRect, headImage);
                    marker.rect(overMark, markColor, 3);
                    isMarked = true;
                }
            }
            if (bodyColor.isValid())
            {
                marker.drawImage(bodyRect, grabRgb.copy(bodyRect));
                marker.rect(bodyRect, bodyColor, 2);
                isMarked = true;
            }
            if (optMarkClothes->toBool())
            {
                marker.drawImage(clothesMatcher->upperRect(), clothesMatcher->upperImage());
                marker.title(clothesMatcher->upperRect(), clothesMatcher->upperColorNames());
                marker.drawImage(clothesMatcher->lowerRect(), clothesMatcher->lowerImage());
                marker.title(clothesMatcher->lowerRect(), clothesMatcher->lowerColorNames());
                isMarked = true;
            }
            if (eyeColor.isValid())
            {
                marker.eyes(eyeLine.translated(crop.topLeft()), eyeColor, 3);
                isMarked = true;
            }
            if (false)
            {
                Eyes eyes(eyeLine);
                marker.horizontalLine(eyes.topOfHead() + crop.topLeft(),
                                      eyes.distance(), Qt::blue, 3);
                isMarked = true;
            }
        } // marked image
    } // else (Template Generation good)

    if (optResolveEnable->toBool())
    {
        resolver->ignore("SearchTemplate");
        int resConf = resolver->confidence();
        int minConf = optResolveMin->toInt();
        int maxConf = optResolveMax->toInt();
        INFO("   Resolver Confidence = %1", resConf);
        QStringList qsl = resolver->report();
        foreach(QString s, qsl)
            INFO("   + %1", s);
        if ((0 == minConf || resConf > minConf) && (0 == maxConf || resConf < maxConf))
        {
            if (fwpResolveMarked->isActive())
            {
                QImage markedImage(grabRgb);
                QColor faceColor = optMarkFaceColor->value<QColor>();
                DDT::ImageMarker marker(&markedImage);
                marker.rect(head, faceColor, 3);
                marker.end();
                fwpResolveMarked->write(markedImage, idGenerator.face("ResolveMarked"));
            }

            if (fwpResolveFace->isActive())
            {
                DDT::QQRect overRect(head);
                overRect = overRect.scaled(optMarkOverCrop->toDouble() / 100.0);
                overRect &= markedImage.rect();
                QImage headImage = grabImage.copy(overRect);
                EXPECT(true,  ! headImage.isNull());
                EXPECT(true, markedImage.rect().contains(overRect));
                fwpResolveFace->write(headImage, idGenerator.face("ResolveFace"));
            }
        }
    }

    if (writeFaceInfo)
    {
        // TODO: append faCapture to frameAttributes
    }
    appSettings->setValue("Output/FacesProcessed",
                          QString::number(++FacesProcessed));

    if ( ! pendingFaces.isEmpty())
        QTimer::singleShot(10, this, SLOT(processFace()));
} // processFace()

