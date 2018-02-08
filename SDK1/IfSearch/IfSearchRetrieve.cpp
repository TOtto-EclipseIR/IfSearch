#include "IfSearch.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <ImageCache.h>
#include <InfoMacros.h>
#include <InputHotdir.h>
#include <Return.h>
#include <Settings.h>

void IfSearch::doPersonReport(void)
{
    PROGRESS("[RETRIEVE] Started for PersonReport");
    retrieveResults.clear();
    appSettings->setValue("Retrieve/Status", "Checking FaceBase");
    appSettings->setValue("Retrieve/Results", retrieveResults);
    appSettings->setValue("Retrieve/Reason", QString());

    if ( ! faceBase)
        return retrieveError("No FaceBase");
    QString fn(appSettings->value("Retrieve/ReportFile").toString());
    if (fn.isEmpty())
        return retrieveError("No ReportFile name specified");
    QFileInfo fi(fn);
    if (0 == fi.suffix().compare("XML", Qt::CaseInsensitive))
        return retrieveError("XML format not yet supported");

    QFile reportFile(fi.filePath());
    if ( ! reportFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return retrieveError(tr("Error opening %1 for truncated write: %2")
                                .arg(fi.filePath())
                                .arg(reportFile.errorString()));
    QTextStream reportStream(&reportFile);
    QList<int> personKeyList = faceBase->personKeys();
    if (personKeyList.isEmpty())
        return retrieveError("No Persons in FaceBase");

    appSettings->setValue("Retrieve/Status", "Writing ReportFile");
    foreach(int pk, personKeyList)
    {
        reportStream << pk;
        reportStream << "," << '"' << faceBase->personId(pk) << '"';
        reportStream << "," << faceBase->personFaceKeys(pk).size();
        reportStream << "," << faceBase->personActive(pk);
        reportStream << "," << '"' << faceBase->personDescription(pk) << '"';
        reportStream << "\r\n";
    }

    reportStream << -1;
    reportStream << "," << '"' << "done" << '"';
    reportStream << "," << 0;
    reportStream << "," << 0;
    reportStream << "," << '"' << " " << '"';
    reportStream << endl;
    reportFile.close();
    retrieveResults = tr("%1 bytes written to %2 for %3 persons")
                      .arg(reportFile.size())
                      .arg(fi.filePath())
                      .arg(personKeyList.size());
    QTimer::singleShot(10, this, SLOT(retrieveFinish()));
}

void IfSearch::doStatus(void)
{
    PROGRESS("[RETRIEVE] Started for Status");
    retrieveResults.clear();
    appSettings->setValue("Retrieve/Status", "Checking FaceBase");
    appSettings->setValue("Retrieve/Results", retrieveResults);
    appSettings->setValue("Retrieve/Reason", QString());

    if ( ! faceBase)
        return retrieveError("No FaceBase");
    Return rtn = faceBase->status();
    RETURN(rtn);
    if (rtn.isError())
        return retrieveError("Status Retrieve error: "+rtn.toString());

    retrieveResults = tr("%1,%2,%3,%4").arg(rtn[1].toInt())
                                       .arg(rtn[2].toInt())
                                       .arg(rtn[3].toInt())
                                       .arg(rtn[4].toInt());
    QTimer::singleShot(10, this, SLOT(retrieveFinish()));
}

void IfSearch::doRetrieve(void)
{
    PROGRESS("[RETRIEVE] Started for Retrieve");
    retrieveResults.clear();
    appSettings->setValue("Retrieve/Status", "Checking FaceBase");
    appSettings->setValue("Retrieve/Results", retrieveResults);
    appSettings->setValue("Retrieve/Reason", QString());

    if ( ! faceBase)
        return retrieveError("No FaceBase");

    retrievePersonKey = appSettings->value("Retrieve/PersonKey", 0).toInt();
    retrievePersonId = appSettings->value("Retrieve/PersonId", QString()).toString();
    retrieveFaceKey = appSettings->value("Retrieve/FaceKey", 0).toInt();
    retrieveFaceId = appSettings->value("Retrieve/FaceId", QString()).toString();
    if (retrievePersonKey)
    {
        retrievePersonId = faceBase->personId(retrievePersonKey);
        if (retrievePersonId.isEmpty())
            return retrieveNotFound(tr("PersonKey %1 does not exist").arg(retrievePersonKey));
    }
    else
    {
        if ( ! retrievePersonId.isEmpty())
        {
            retrievePersonKey = faceBase->personKey(retrievePersonId);
            if ( ! retrievePersonKey)
                return retrieveNotFound(tr("PersonId %1 does not exist").arg(retrievePersonId));
        }
    }
    if (retrieveFaceKey)
    {
        retrieveFaceId = faceBase->faceId(retrieveFaceKey, retrievePersonKey);
    }
    else
    {
        if ( ! retrieveFaceId.isEmpty())
        {
            retrieveFaceKey = faceBase->faceKey(retrieveFaceId, retrievePersonKey);
            if ( ! retrieveFaceKey)
                return retrieveNotFound(tr("FaceId %1 does not exist").arg(retrieveFaceId));
        }
    }
    if (retrieveFaceKey)
        retrievePersonKey = faceBase->personKey(retrieveFaceKey);
    if (retrieveFaceId.isEmpty() && retrieveFaceKey)
        retrieveFaceId = faceBase->faceId(retrieveFaceKey, retrievePersonKey);
    if (retrievePersonId.isEmpty() && retrievePersonKey)
        retrievePersonId = faceBase->personId(retrievePersonKey);

    if (retrieveFaceKey)
        QTimer::singleShot(10, this, SLOT(retrieveFaceOnly()));
    else if (retrievePersonKey)
        QTimer::singleShot(10, this, SLOT(retrievePerson()));
    else
        return retrieveError(tr("Person or Face Id or Key must be supplied for Retrieve"));
} // doRetrieve()

void IfSearch::doRetrieveAll(void)
{
    if ( ! fwpRetrieve->isActive()) return retrieveError("No Retrieve/OutputDir");
    if ( ! faceBase) return retrieveError("No FaceBase");
    bool activeOnly = appSettings->value("Retrieve/ActiveOnly", true).toBool();

    foreach (int pKey, faceBase->personKeys())
    {
        QString pId(faceBase->personId(pKey));
        appSettings->setValue("Retrieve/Status", "Writing "+pId);
        QString personDirName = tr("PK%1-%2/").arg(pKey, 6, 10, QChar('0')).arg(pId);
        QString personFileName = faceBase->personFilePath(pKey);
        fwpRetrieve->write(QFile(personFileName), personDirName+"Person");
        foreach (int faceKey, faceBase->personFaceKeys(pKey))
        {
            bool active = faceBase->isActive(faceKey);
            if (active || ! activeOnly)
            {
                QFileInfo enrolledFileInfo(faceBase->enrolledImageName(faceKey));
                fwpRetrieve->write(QFile(enrolledFileInfo.absoluteFilePath()),
                                   tr("%2/%1").arg(enrolledFileInfo.completeBaseName())
                                              .arg(personDirName));
                if (fwpRetrRecon->isActive())
                {
                    EigenFaceFace eff;
                    faceBase->getFace(&eff, faceKey);
                    EigenFaceTemplate eft = eff.templat();
                    QImage recon = eigenFace->getReconImage(&eft);
                    fwpRetrRecon->write(recon, tr("%2/%1R")
                                       .arg(enrolledFileInfo.completeBaseName())
                                       .arg(personDirName));
                }
            }
        } // foreach(personFaceKeys)
    } // foreach(personKeys)

    appSettings->setValue("Retrieve/Status", "Writing Anonymous");
    QString personDirName = tr("PK%1-%2/").arg(0, 6, 10, QChar('0')).arg("Anonymous");
    foreach (int faceKey, faceBase->personFaceKeys(0))
    {
        bool active = faceBase->isActive(faceKey);
        if (active || ! activeOnly)
        {
            QString enrolledFileName(faceBase->enrolledImageName(faceKey));
            fwpRetrieve->write(QFile(enrolledFileName), personDirName+enrolledFileName);
        }
    } // foreach(anonymous)

    Return rtn = faceBase->status();
    if (rtn.isError()) return retrieveError("Status Retrieve error: "+rtn.toString());
    retrieveResults = tr("%1,%2,%3,%4").arg(rtn[0].toInt())
                                       .arg(rtn[1].toInt())
                                       .arg(rtn[2].toInt())
                                       .arg(rtn[3].toInt());
    QTimer::singleShot(10, this, SLOT(retrieveFinish()));
} // doRetrieveAll()

void IfSearch::retrievePerson(void)
{
    if ( ! faceBase)
        return retrieveError("No FaceBase");

    bool activeOnly = appSettings->value("Retrieve/ActiveOnly", true).toBool();

    retrieveResults = tr("Person: %1=%2")
                        .arg(retrievePersonKey, 6, 10, QChar('0'))
                        .arg(retrievePersonId);

    if (fwpRetrieve->isActive())
    {
        QString personFileName = faceBase->personFilePath(retrievePersonKey);
        fwpRetrieve->write(QFile(personFileName));
    }

    QList<int> faceKeys = faceBase->personFaceKeys(retrievePersonKey);
    foreach (int faceKey, faceKeys)
    {
        QString faceId = faceBase->faceId(faceKey, retrievePersonKey);
        bool active = faceBase->isActive(faceKey);
        retrieveResults += resultsDelimiter + tr("   Face: %1=%2 {%3}")
                           .arg(faceKey, 6, 10, QChar('0'))
                           .arg(faceId)
                           .arg(faceBase->statusName(faceKey));
        if (fwpRetrieve->isActive() && (active || ! activeOnly))
        {
            QString enrolledFileName = faceBase->enrolledImageName(faceKey);
            fwpRetrieve->write(QFile(enrolledFileName));
        }
    } // foreach(faceKeys)
    appSettings->setValue("Retrieve/Description",
                          faceBase->personDescription(retrievePersonKey));

    QTimer::singleShot(10, this, SLOT(retrieveFinish()));
} // retrievePerson()

void IfSearch::retrieveFaceOnly(void)
{
    if ( ! faceBase)
        return retrieveError("No FaceBase");

    bool activeOnly = appSettings->value("Retrieve/ActiveOnly", true).toBool();
    bool active = faceBase->isActive(retrieveFaceKey);
    retrieveResults += tr("   Face: %1=%2 {%3}")
                       .arg(retrieveFaceKey, 6, 10, QChar('0'))
                       .arg(retrieveFaceId)
                       .arg(faceBase->statusName(retrieveFaceKey));

    if (fwpRetrieve->isActive() && (active || ! activeOnly))
    {
        QString enrolledFileName = faceBase->enrolledImageName(retrieveFaceKey);
        fwpRetrieve->write(QFile(enrolledFileName));
    }

    QTimer::singleShot(10, this, SLOT(retrieveFinish()));
} // retrieveFaceOnly()

void IfSearch::retrieveFinish(void)
{
    if (fwpRetrieve->hasPending())
    {
        INFO("[RETRIEVE] %1 writes pending", fwpRetrieve->sizePending());
        QTimer::singleShot(100, this, SLOT(retrieveFinish()));
        return;
    }

    QString Description = faceBase->personDescription(enrollPersonKey);
    appSettings->setValue("Retrieve/Description", Description);

    PROGRESS("[RETRIEVE] Done - Results:");
    QStringList qsl = retrieveResults.split(resultsDelimiter);
    foreach (QString s, qsl)
        INFO("\t%1", s);
    appSettings->setValue("Retrieve/Results", retrieveResults);
    appSettings->setValue("Retrieve/Reason", QString());
    appSettings->setValue("Retrieve/Status", "Done");
    appSettings->setValue("Retrieve/Command", QString());
    Info::flush();
    commandMode.clear();
} // retrieveFinish()

void IfSearch::retrieveError(const QString & reason)
{
    ERRMSG("[RETRIEVE] Error: %1", reason);
    appSettings->setValue("Retrieve/Reason", reason);
    appSettings->setValue("Retrieve/Status", "Error");
    appSettings->setValue("Retrieve/Command", QString());
    Info::flush();
    commandMode.clear();
}

void IfSearch::retrieveNotFound(const QString & reason)
{
    ERRMSG("[RETRIEVE] NotFound: %1", reason);
    appSettings->setValue("Retrieve/Reason", reason);
    appSettings->setValue("Retrieve/Status", "NotFound");
    appSettings->setValue("Retrieve/Command", QString());
    Info::flush();
    commandMode.clear();
}
