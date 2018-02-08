#include "IfSearch.h"

#include <QDir>
#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <EigenFaceSearchResultList.h>
#include <EigenFaceSearchSettings.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <ImageCache.h>
#include <ImageMarker.h>
#include <InfoMacros.h>
#include <InputHotdir.h>
#include <Return.h>
#include <Settings.h>

void IfSearch::doSimilarity(void)
{
    if (ffdBusy)
    {
        INFO("[SEARCH] ffdBusy");
        if (--searchTimeout <= 0)
            return searchError("Timeout");
        QTimer::singleShot(100, this, SLOT(doSearch()));
        return;
    }
    PROGRESS("[SEARCH] Started for Similarity");
    searchResults.clear();
    appSettings->setValue("Search/Results", searchResults);
    appSettings->setValue("Search/Reason", QString());
    appSettings->setValue("Search/Status", "Starting");

    if (searchSettings.getPersonMode())
        return searchError("Similarity in PersonMode not supported");

    searchSimilarity->clear();
    QTimer::singleShot(10, this, SLOT(similarityScanDir()));
}

void IfSearch::similarityScanDir(void)
{
    NULLPTR(appSettings); NULLPTR(writer);

    appSettings->setValue("Search/Status", "Scanning Directory");
    QString dirName = appSettings->value("Search/InputDir", "../Search").toString();
    if (dirName.isEmpty())
        return searchError("Search/InputDir is empty");

    searchDir = writer->baseDir();
    if ( ! searchDir.cd(dirName))
        return searchError(tr("Can't cd to %1").arg(dirName));
    searchPending = searchDir.entryList(QDir::Files);
    if (searchPending.isEmpty())
        return searchError(tr("%1 is empty").arg(dirName));

    appSettings->setValue("Search/Status", "Reading Files");
    QTimer::singleShot(10, this, SLOT(similarityGrab()));
} // similarityScanDir()

void IfSearch::similarityGrab(void)
{
    if (searchPending.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(similarityProcess()));
        return;
    }

    QString fileName = searchPending.takeFirst();
    EigenFaceTemplate tpl;
    DETAIL("Reading %1", searchDir.absoluteFilePath(fileName));
    QImage faceImage(searchDir.absoluteFilePath(fileName));

    Return rtn = tpl.read(faceImage);
    if (rtn.isError())
        return searchError(rtn.toString());

    QFileInfo fi(searchDir.absoluteFilePath(fileName));
    int faceKey = searchSimilarity->insert(tpl, fi.completeBaseName(), faceImage);
    searchResults += resultsDelimiter + tr("%1 FK%2 %3 Similarity Face Active")
            .arg(searchSimilarity->size())
            .arg(faceKey, 6, 10, QChar('0'))
            .arg(fi.completeBaseName());

    QTimer::singleShot(10, this, SLOT(similarityGrab()));
} // similarityGrab()

void IfSearch::similarityProcess(void)
{
    Return rtn;

    if (searchSimilarity->size() < 2)
        return searchError("Not enough consistent Faces for Similarity");

    appSettings->setValue("Search/Status", "Processing");
    rtn = searchSimilarity->process(&similarityResultsList);
    if (rtn.isError())
        return searchError(rtn.toString());

    appSettings->setValue("Search/Status", "Writing Results");
    QTimer::singleShot(10, this, SLOT(similarityResults()));
} // similarityProcess()

void IfSearch::similarityResults(void)
{
    Return rtn;

    if (similarityResultsList.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(searchFinish()));
        return;
    }

    EigenFaceSimilarityResult result = similarityResultsList.takeFirst();
    EigenFaceSimilarityEntry entry = result.first();
    searchResults += resultsDelimiter
            + tr("%1. S%2%3 FK%4~FK%5")
            .arg(result.getRank(), 2)
            .arg(result.getConfidence(), 3, 10, QChar('0'))
            .arg(result.getTier().indicator())
            .arg(entry.getFaceKey1(), 6, 10, QChar('0'))
            .arg(entry.getFaceKey2(), 6, 10, QChar('0'));

    if (fwpSimilarity->isActive())
    {
        QImage topImage(searchSimilarity->image(entry.getFaceKey1()));
        QImage botImage(searchSimilarity->image(entry.getFaceKey2()));
        ImageMarker topMarker(&topImage);
        ImageMarker botMarker(&botImage);
        topMarker.title(tr("#%1 %2")
                        .arg(entry.getFaceKey1())
                        .arg(entry.getFaceId1()));
        topMarker.score(result.getConfidence(), 6,
                        result.getTier().color());
        botMarker.title(tr("#%1 %2")
                        .arg(entry.getFaceKey2())
                        .arg(entry.getFaceId2()));
        topMarker.end();
        botMarker.end();

        QImage outImage(qMax(topImage.width(), botImage.width()),
                        topImage.height() + botImage.height(),
                        QImage::Format_RGB32);
        ImageMarker outMarker(&outImage);
        outMarker.drawImage(0, 0, topImage);
        outMarker.drawImage(0, topImage.height(), botImage);
        outMarker.end();

        QString fileName = tr("R%1-S%2%3-FK%4-%5~FK%6-%7")
                .arg(result.getRank(), 2, 10, QChar('0'))
                .arg(result.getConfidence(), 3, 10, QChar('0'))
                .arg(result.getTier().indicator())
                .arg(entry.getFaceKey1(), 6, 10, QChar('0'))
                .arg(entry.getFaceId1())
                .arg(entry.getFaceKey2(), 6, 10, QChar('0'))
                .arg(entry.getFaceId2());
        fwpSimilarity->write(outImage, fileName);
    }

    QTimer::singleShot(10, this, SLOT(similarityResults()));
} // similarityResults()
