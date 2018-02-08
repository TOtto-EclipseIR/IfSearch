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
    Return rtn = tpl.read(searchDir.absoluteFilePath(fileName));
    if (rtn.isError())
        return searchError(rtn.toString());

    QFileInfo fi(searchDir.absoluteFilePath(fileName));
    int faceKey = searchSimilarity->insert(tpl, fi.completeBaseName());
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
            + tr("%1. M%2%3 FK%4~FK%5")
            .arg(result.getRank(), 2)
            .arg(result.getConfidence(), 3, 10, QChar('0'))
            .arg(result.getTier().indicator())
            .arg(entry.getFaceKey1(), 6, 10, QChar('0'))
            .arg(entry.getFaceKey2(), 6, 10, QChar('0'));

    QTimer::singleShot(10, this, SLOT(similarityResults()));
} // similarityResults()
