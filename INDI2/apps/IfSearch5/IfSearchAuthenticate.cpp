#include "IfSearch.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <EigenFaceSearchResultList.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <ImageCache.h>
#include <InfoMacros.h>
#include <InputHotdir.h>
#include <Return.h>
#include <Settings.h>

void IfSearch::doAuthenticate(void)
{
    if (ffdBusy)
    {
        INFO("[SEARCH] ffdBusy");
        if (--searchTimeout)
            return searchError("Timeout");
        QTimer::singleShot(100, this, SLOT(doAuthenticate()));
        return;
    }
    searchResults.clear();
    authConfidence = -1;
    authUpdate = 0;
    appSettings->setValue("Search/Results", searchResults);
    appSettings->setValue("Search/Reason", QString());
    appSettings->setValue("Search/Confidence", authConfidence = 0);
    searchPersonKey = appSettings->value("Search/PersonKey", 0).toInt();
    searchPersonId = appSettings->value("Search/PersonId", QString()).toString();
    PROGRESS("[SEARCH] Started for Authenticate: %1 #%2", searchPersonId, searchPersonKey);

    searchTemplate.clear();
    appSettings->setValue("Search/Status", "Checking FaceBase");
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
            return searchError(tr("PersonId or PersonKey must be specified for Authenticate"));
        searchPersonKey = faceBase->personKey(searchPersonId);
        if ( ! searchPersonKey)
            return searchError(tr("PersonId %1 does not exist").arg(searchPersonId));
    }

    NULLPTR(appSettings); NULLPTR(writer);
    QStringList qsl;

    appSettings->setValue("Search/Status", "Scanning Directory");
    QString dirName = appSettings->value("Search/InputDir", "../Search").toString();
    if (dirName.isEmpty())
        return searchError("Search/InputDir is empty");

    searchDir = writer->baseDir();
    if ( ! searchDir.cd(dirName))
        return searchError(tr("Can't cd to %1").arg(dirName));
    searchPending = searchDir.entryList(QDir::Files);
    authWatcher = new QFileSystemWatcher(QStringList() << searchDir.absolutePath(), this);
    connect(authWatcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(authDirChange(QString)));

    searchResults = tr("Authenticating %1:%2").arg(searchPersonKey).arg(searchPersonId);
    appSettings->setValue("Search/Status", "Collecting");

    int toMsec = appSettings->value("Search/AuthTimeout", 5000).toInt();
    QTimer::singleShot(toMsec, this, SLOT(authFinished()));
    QTimer::singleShot(10, this, SLOT(tryAuthenticate()));
} // doAuthenticate()

void IfSearch::authDirChange(QString path)
{
    searchPending = searchDir.entryList(QDir::Files);
    if ( ! searchTemplate.isEmpty() && searchPending.isEmpty())
    {
        searchResults += resultsDelimiter + "Cancelled";
        authFinished();
    }
    else
        QTimer::singleShot(10, this, SLOT(tryAuthenticate()));
}

void IfSearch::tryAuthenticate(void)
{
    foreach (QString id, searchTemplate.idList())
    {
        if (searchPending.contains(id))
            searchPending.removeAll(id);
        else
        {
            searchTemplate.removeById(id);
            searchResults += resultsDelimiter + tr("%1 Removed").arg(id);
        }
    }

    foreach (QString fileName, searchPending)
    {
        EigenFaceTemplate tpl;
        Return rtn = tpl.read(searchDir.absoluteFilePath(fileName));
        if (rtn.isError()) return searchError(rtn.toString());
        tpl.setVectorId(fileName);
        searchTemplate.add(tpl);
        searchResults += resultsDelimiter + tr("%1 Active").arg(fileName);
    }

    int minFaces = appSettings->value("Search/AuthMinFaces", 3).toInt();
    int maxFaces = appSettings->value("Search/AuthMaxFaces", 10).toInt();
    int passConfidence = appSettings->value("Search/AuthPassConfidence", 850).toInt();
    if (searchTemplate.size() >= minFaces)
    {
        QList<int> faceKeys = faceBase->personFaceKeys(searchPersonKey);
        EigenFaceSearchResultList resList
                = eigenMatcher->search(EigenFaceSearchSettings::Authenticate,
                                       searchTemplate,
                                       &searchSettings,
                                       faceKeys);
        authConfidence = resList.bestConfidence();
        searchResults += resultsDelimiter + tr("Confidence=%1").arg(authConfidence);
        appSettings->setValue("Search/Confidence", authConfidence);
        appSettings->setValue("Search/Status", tr("Update %1").arg(++authUpdate));
        if (authConfidence >= passConfidence)
        {
            searchResults += resultsDelimiter + "Pass";
            authFinished();
        }
        if (searchTemplate.size() >= maxFaces)
        {
            searchResults += resultsDelimiter + "MaxFaces";
            authFinished();
        }
    }
}

void IfSearch::authFinished(void)
{
    delete authWatcher;
    authWatcher = 0;
    int failConfidence = appSettings->value("Search/AuthFailConfidence", 600).toInt();
    int passConfidence = appSettings->value("Search/AuthPassConfidence", 850).toInt();
    if (authConfidence < failConfidence)
        appSettings->setValue("Search/Authenticate", "Fail");
    else if (authConfidence >= passConfidence)
        appSettings->setValue("Search/Authenticate", "Pass");
    else
        appSettings->setValue("Search/Authenticate", "Maybe");
    return searchFinish();
}
