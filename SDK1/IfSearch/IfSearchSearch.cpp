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

void IfSearch::doSearch(void)
{
    if (ffdBusy)
    {
        INFO("[SEARCH] ffdBusy");
        if (--searchTimeout <= 0)
            return searchError("Timeout");
        QTimer::singleShot(100, this, SLOT(doSearch()));
        return;
    }
    PROGRESS("[SEARCH] Started for Search");
    searchResults.clear();
    appSettings->setValue("Search/Results", searchResults);
    appSettings->setValue("Search/Reason", QString());
    searchPersonKey = 0;
    searchPersonId.clear();
    searchTemplate.clear();
    QTimer::singleShot(10, this, SLOT(searchScanDir()));
}

void IfSearch::doVerify(void)
{
    if (ffdBusy)
    {
        INFO("[SEARCH] ffdBusy");
        if (--searchTimeout)
            return searchError("Timeout");
        QTimer::singleShot(100, this, SLOT(doVerify()));
        return;
    }
    searchResults.clear();
    appSettings->setValue("Search/Results", searchResults);
    appSettings->setValue("Search/Reason", QString());
    searchPersonKey = appSettings->value("Search/PersonKey", 0).toInt();
    searchPersonId = appSettings->value("Search/PersonId", QString()).toString();
    searchTemplate.clear();
    appSettings->setValue("Search/Status", "Checking FaceBase");
    PROGRESS("[SEARCH] Started for Verify: %1 #%2", searchPersonId, searchPersonKey);

    if ( ! faceBase)
        return searchError("No FaceBase");

    if (searchPersonKey)
    {
        searchPersonId = faceBase->personId(searchPersonKey);
        if (searchPersonId.isEmpty())
            return searchError(tr("PersonKey %1 does not exist").arg(searchPersonKey));
    }
    else
    {
        if (searchPersonId.isEmpty())
            return searchError(tr("PersonId or PersonKey must be specified for Verify"));
        searchPersonKey = faceBase->personKey(searchPersonId);
        if ( ! searchPersonKey)
            return searchError(tr("PersonId %1 does not exist").arg(searchPersonId));
    }

    QTimer::singleShot(10, this, SLOT(searchScanDir()));
} // doVerify()

void IfSearch::searchScanDir(void)
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
    QTimer::singleShot(10, this, SLOT(searchGrab()));
} // searchScanDir()

void IfSearch::searchGrab(void)
{
    if (searchPending.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(searchOrVerify()));
        return;
    }

    QString fileName = searchPending.takeFirst();
    EigenFaceTemplate tpl;
    Return rtn = tpl.read(searchDir.absoluteFilePath(fileName));
    if (rtn.isError())
        return searchError(rtn.toString());

    searchTemplate.add(tpl);
    searchResults += resultsDelimiter + tr("%1 search image Active")
                                           .arg(fileName);

    QTimer::singleShot(10, this, SLOT(searchGrab()));
} // searchGrab()

void IfSearch::searchOrVerify(void)
{
    Return rtn;

    if (searchTemplate.isEmpty())
        return searchError("No Consistent Faces to search");
    if ( ! faceBase)
        return searchError("No FaceBase");

    if (searchPersonKey)
    {
        // verify
        appSettings->setValue("Search/Status", "Verifying");
        QList<int> faceKeys = faceBase->personFaceKeys(searchPersonKey);
        EigenFaceSearchResultList resList
                = eigenMatcher->search(EigenFaceSearchSettings::Verify,
                                       searchTemplate,
                                       &searchSettings,
                                       faceKeys);
        int confidence = resList.bestConfidence();
        EigenFaceSearchTier tier = resList.bestTier();
        searchResults += resultsDelimiter + tr("Confidence=%1 %2")
                .arg(confidence).arg(tier.name());
    }
    else
    {
        appSettings->setValue("Search/Status", "Searching");
        EigenFaceSearchResultList resList
                = eigenMatcher->search(EigenFaceSearchSettings::FormalSearch,
                                   searchTemplate,
                                   &searchSettings);
        searchResults += resultsDelimiter + tr("%1 results").arg(resList.size());
        int rank = 0;
        foreach (EigenFaceSearchPerson res, resList)
        {
            ++rank;
            QString personId = faceBase->personId(res.getPersonKey());
            searchResults += resultsDelimiter + tr("%1. %2 %5 %3 %4")
                             .arg(rank, 2)
                             .arg(res.getConfidence(), 3)
                             .arg(res.getPersonKey(), 8)
                             .arg(personId)
                             .arg(res.getTier().name(), 10);
            if (fwpSearch->isActive())
            {
                QString dirName = tr("R%1M%2%4-%3/").arg(rank, 2, 10, QChar('0'))
                                    .arg(res.getConfidence(), 3, 10, QChar('0'))
                                    .arg(personId).arg(res.getTier().indicator());
                foreach (EigenFaceSearchResult faceResult, res.results())
                {
                    QString enrolledFileName = faceBase->enrolledImageName(faceResult.getFaceKey());
                    QString baseName = tr("R%1M%2-FK%3").arg(faceResult.getRank(), 2, 10, QChar('0'))
                                        .arg(faceResult.getConfidence(), 3, 10, QChar('0'))
                                        .arg(faceResult.getFaceKey(), 6, 10, QChar('0'));
                    fwpSearch->write(QFile(enrolledFileName), dirName + baseName);
                } // foreach(faceResult)
            }
        } // foreach(res)
    } // else (personKey)

    QTimer::singleShot(10, this, SLOT(searchFinish()));
} // searchOrVerify()

void IfSearch::searchFinish(void)
{
    if (fwpSearch->hasPending())
    {
        INFO("[SEARCH] %1 writes pending for Search", fwpSearch->sizePending());
        QTimer::singleShot(100, this, SLOT(searchFinish()));
        return;
    }
    if (fwpSimilarity->hasPending())
    {
        INFO("[SEARCH] %1 writes pending for Simularity", fwpSimilarity->sizePending());
        QTimer::singleShot(100, this, SLOT(searchFinish()));
        return;
    }

    PROGRESS("[SEARCH] Done - Results:");
    QStringList qsl = searchResults.split(resultsDelimiter);
    foreach (QString s, qsl)
        INFO("\t%1", s);
    appSettings->setValue("Search/Results", searchResults);
    appSettings->setValue("Search/Reason", QString());
    appSettings->setValue("Search/Status", "Done");
    appSettings->setValue("Search/Command", QString());
    Info::flush();
    commandMode.clear();
} // searchFinish()

void IfSearch::searchError(const QString & reason)
{
    ERRMSG("[SEARCH] Error: %1", reason);
    appSettings->setValue("Search/Results", QString());
    appSettings->setValue("Search/Reason", reason);
    appSettings->setValue("Search/Status", "Error");
    appSettings->setValue("Search/Command", QString());
    Info::flush();
    commandMode.clear();
} // searchError()
