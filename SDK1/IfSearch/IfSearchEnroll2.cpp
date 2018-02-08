#include "IfSearch.h"

#include <QImageReader>
#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <FaceBase.h>
#include <FileWriteProfile.h>
#include <FileWriter.h>
#include <fbPerson.h>
#include <ImageCache.h>
#include <InfoMacros.h>
#include <InputHotdir.h>
#include <Return.h>
#include <Settings.h>


void IfSearch::doDelete(void)
{
    PROGRESS("[ENROLL] Started for Delete Face");
    enrollResults.clear();
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());

    if ( ! faceBase)
        return enrollError("No FaceBase");

    int faceKey = appSettings->value("Enroll/FaceKey", 0).toInt();
    QString faceId = appSettings->value("Enroll/FaceId", QString()).toString();
    if (faceKey)
    {
        enrollPersonKey = 0;
        enrollPersonId.clear();
        faceId.clear();
        if ( ! faceBase->containsFace(faceKey))
            return enrollError(tr("FaceKey %1 does not exist").arg(faceKey));
    }
    else
    {
        if (faceId.isEmpty())
            return enrollError(tr("FaceId or FaceKey must be specified"));

        enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
        enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
        if (enrollPersonKey)
        {
            enrollPersonId = faceBase->personId(enrollPersonKey);
            if (enrollPersonId.isEmpty())
                return enrollError(tr("PersonKey %1 does not exist").arg(enrollPersonKey));
        }
        else
        {
            if ( ! enrollPersonId.isEmpty())
            {
                enrollPersonKey = faceBase->personKey(enrollPersonId);
                if ( ! enrollPersonKey)
                    return enrollError(tr("PersonId %1 does not exist").arg(enrollPersonId));
            }
        }
        faceKey = faceBase->faceKey(faceId, enrollPersonKey);
        if ( ! faceKey)
            return enrollError(enrollPersonKey
                               ? tr("FaceId %1 does not exist for %2").arg(faceId).arg(enrollPersonId)
                               : tr("FaceId %1 does not exist unidentfied").arg(faceId));
    }

    Return rtn = faceBase->deleteFace(faceKey);
    if (rtn.isError())
        return enrollError(rtn.toString());

    if (faceId.isEmpty())
        enrollResults = tr("Deleted FaceKey=%1").arg(faceKey);
    else
        enrollResults = tr("Deleted FaceKey=%1 FaceId=%2 PersonKey=%3 PersonId=%4")
                    .arg(faceKey).arg(faceId).arg(enrollPersonKey).arg(enrollPersonId);
    QTimer::singleShot(10, this, SLOT(enrollFinish()));
} // doDelete()

void IfSearch::doRemove(void)
{
    PROGRESS("[ENROLL] Started for Remove Person");
    enrollResults.clear();
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());

    if ( ! faceBase)
        return enrollError("No FaceBase");

    enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
    enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
    if (enrollPersonKey)
    {
        enrollPersonId = faceBase->personId(enrollPersonKey);
        if (enrollPersonId.isEmpty())
            return enrollError(tr("PersonKey %1 does not exist").arg(enrollPersonKey));
    }
    else
    {
        if ( ! enrollPersonId.isEmpty())
        {
            enrollPersonKey = faceBase->personKey(enrollPersonId);
            if ( ! enrollPersonKey)
                return enrollError(tr("PersonId %1 does not exist").arg(enrollPersonId));
        }
    }
    if ( ! enrollPersonKey)
        return enrollError(tr("Must specify person key or id to remove"));

    Return rtn = faceBase->removePerson(enrollPersonKey);
    if (rtn.isError())
        return enrollError(rtn.toString());

    enrollResults = tr("Removed PersonKey=%1 PersonId=%2")
                    .arg(enrollPersonKey).arg(enrollPersonId);
    QTimer::singleShot(10, this, SLOT(enrollFinish()));
} // doRemove()

void IfSearch::doRemoveAll(void)
{
    PROGRESS("[ENROLL] Started for Remove All");
    enrollResults.clear();
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());

    if ( ! faceBase)
        return enrollError("No FaceBase");

    enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
    if (0 == enrollPersonId.compare("Confirm", Qt::CaseInsensitive))
    {
        appSettings->setValue("Enroll/Status", "Inactivating Enrollment");
        Return rtn = faceBase->removeAll();
        if (rtn.isError())
            return enrollError(rtn.toString());
        enrollResults = tr("RemovedAll");
    }
    else if (0 == enrollPersonId.compare("Destroy", Qt::CaseInsensitive))
    {
        appSettings->setValue("Enroll/Status", "Destroying Enrollment Directory");
        Return rtn = faceBase->destroy();
        if (rtn.isError())
            return enrollError(rtn.toString());
        enrollResults = tr("RemovedAll");
    }
    else
        return enrollError(tr("RemoveAll not confirmed: %1").arg(enrollPersonId));

    QTimer::singleShot(10, this, SLOT(doReload()));
} // doRemoveAll()

void IfSearch::doRename(void)
{
    PROGRESS("[ENROLL] Started for Rename Person");
    enrollResults.clear();
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());

    if ( ! faceBase)
        return enrollError("No FaceBase");

    enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
    enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
    if (enrollPersonKey)
    {
        enrollPersonId = faceBase->personId(enrollPersonKey);
        if (enrollPersonId.isEmpty())
            return enrollError(tr("PersonKey %1 does not exist").arg(enrollPersonKey));
    }
    else
    {
        if ( ! enrollPersonId.isEmpty())
        {
            enrollPersonKey = faceBase->personKey(enrollPersonId);
            if ( ! enrollPersonKey)
                return enrollError(tr("PersonId %1 does not exist").arg(enrollPersonId));
        }
    }
    if ( ! enrollPersonKey)
        return enrollError(tr("Must specify person key or id to rename"));

    QString newPersonId = appSettings->value("Enroll/NewPersonId", QString()).toString();
    if (newPersonId.isEmpty())
        return enrollError(tr("Must specify NewPersonKey for rename"));
    if (faceBase->personKey(newPersonId))
        return enrollError(tr("NewPersonKey %1 already exists"));

    Return rtn = faceBase->renamePerson(enrollPersonKey, newPersonId);
    if (rtn.isError())
        return enrollError(rtn.toString());

    enrollResults = tr("Renamed PersonKey=%1 PersonId=%2")
                    .arg(enrollPersonKey).arg(newPersonId);
    QTimer::singleShot(10, this, SLOT(enrollFinish()));
} // doRename()

void IfSearch::doReload(void)
{
    PROGRESS("[ENROLL] Started for Reload");
    Return rtn = faceBase->initialize(faceBaseBaseDir, faceBaseMaxLoad);
    RETURN(rtn);
    if (rtn.isError() || rtn.isFalse())
        return enrollError(rtn.toString());
    else
    {
        enrollResults = rtn.toString();
        return enrollFinish();
    }
} // doReload()

void IfSearch::reload(void)
{
    PROGRESS("Timer Reload");
    Return rtn = faceBase->initialize(faceBaseBaseDir, faceBaseMaxLoad);
    RETURN(rtn);
} // slot reload()

void IfSearch::doManual(void)
{
    /*--- Initialize ---*/
    PROGRESS("[ENROLL] Started for Manual");
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());
    if ( ! faceBase)
        return enrollError("No FaceBase");
    enrollPersonMode = appSettings->value("Enroll/PersonMode", true).toBool();
    enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
    enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
    QString faceId = appSettings->value("Enroll/FaceId", QString()).toString();
    QString eyeString = appSettings->value("Enroll/EyePoints", QString()).toString();
    INFO("EyePoints=%1", eyeString);
    Eyes manualEyes(eyeString);
    INFO("ManualEyes=%1", manualEyes.toString());
    if (manualEyes.isNull())
        return enrollError("Bad EyePoints: " + eyeString);

    /*--- Look for file ---*/
    appSettings->setValue("Enroll/Status", "Scanning Directory");
    QString dirName = appSettings->value("Enroll/InputDir", "./Enroll").toString();
    if (dirName.isEmpty())
        return enrollError("Enroll/InputDir is blank");
    QDir manualDir = writer->baseDir();
    TRACE("manualDir=%1", manualDir.path());
    if ( ! manualDir.cd(dirName))
        return enrollError(tr("Can't cd to %1 from %2")
                           .arg(dirName).arg(manualDir.absolutePath()));
    appSettings->setValue("Enroll/Status", "Scanning " + manualDir.absolutePath());
    QStringList fileFilter;
    QList<QByteArray> lba = QImageReader::supportedImageFormats();
    foreach(QByteArray ba, lba)
        fileFilter << "*." + ba;
    QStringList manualPending = manualDir.entryList(fileFilter, QDir::Files);
    if (manualPending.isEmpty())
        return enrollError(tr("%1 is empty").arg(dirName));
    appSettings->setValue("Enroll/Status",
                          "Scanned " + QString::number(manualPending.size()));

    QString fileName = manualPending.takeFirst();
    appSettings->setValue("Enroll/Status", "Found " + fileName);
    QFileInfo fi(manualDir.absoluteFilePath(fileName));
    QString imageId = fi.completeBaseName();
    appSettings->setValue("Enroll/Status", "Reading " + imageId);
    if (faceId.isEmpty()) faceId = imageId;
    QImage manualImage(fi.absoluteFilePath());
    if (manualImage.isNull())
        return enrollError(tr("%1 is null image").arg(fi.absoluteFilePath()));

    /*--- Identify Person ---*/
    if (enrollPersonMode)
    {
        if (enrollPersonKey)
        {
            enrollPersonId = faceBase->personId(enrollPersonKey);
            if (enrollPersonId.isEmpty())
                return enrollError(tr("PersonKey %1 does not exist").arg(enrollPersonKey));
        }
        else
        {
            if ( ! enrollPersonId.isEmpty())
            {
                enrollPersonKey = faceBase->personKey(enrollPersonId);
                if ( ! enrollPersonKey)
                {
                    fbPerson person;
                    person.setId(enrollPersonId);
                    Return rtn = faceBase->add(&person);
                    if (rtn.isError())
                        return enrollError(tr("Cannot create new person"));
                    enrollPersonKey = person.key();
                }
            }
        }
    }
    else
    {
        enrollPersonId.clear();
        enrollPersonKey = 0;
    }
    if (faceBase->faceKey(faceId, enrollPersonKey))
        return enrollError(tr("%1 already enrolled").arg(faceId));

    /*--- Generate Template ---*/
    appSettings->setValue("Enroll/Status", "Generating Template");
    eigenFace->clear();
    Return rtn = eigenFace->setImage(manualImage);
    RETURN(rtn);
    eigenFace->setEyePosition(manualEyes);
    rtn = eigenFace->generateTemplate(1);
    RETURN(rtn);
    if (rtn.isError())
        return enrollError(rtn.toString());
    EigenFaceTemplate tpl(eigenFace->templat());
    if (tpl.isEmpty())
        return enrollError("Unable to generate template");

    /*--- Enroll Template ---*/
    EigenFaceStatus status(appSettings->value("Enroll/FaceStatus").toString());
    if ( ! status.isValid())
        status = EigenFaceStatus::Manual;
    appSettings->setValue("Enroll/Status", "Enrolling Template");
    EigenFaceFace face;
    face.setTemplate(tpl);
    face.setStatus(status);
    face.setFaceKey(0);
    face.setPersonKey(enrollPersonKey);
    face.setFaceId(faceId);
    face.setPersonId(enrollPersonId);
    QImage normImage(eigenFace->getNormalImage());
    rtn = faceBase->add(&face, normImage);
    RETURN(rtn);

    /*--- Finish ---*/
    appSettings->setValue("Enroll/Status", "Finished");
    enrollResults += resultsDelimiter
                    + tr("%1 enrolled as %2:%3 %4")
                        .arg(imageId)
                        .arg(face.personKey())
                        .arg(face.faceKey())
                        .arg(face.status().name());
    QTimer::singleShot(10, this, SLOT(enrollFinish()));
} // doManual()

void IfSearch::doReenroll(void)
{
    PROGRESS("[ENROLL] Started for Reenroll");
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());
    if ( ! faceBase)
        return enrollError("No FaceBase");

    int faceKey = appSettings->value("Enroll/FaceKey", 0).toInt();
    QString faceId = appSettings->value("Enroll/FaceId", QString()).toString();
    if (faceKey)
    {
        enrollPersonKey = 0;
        enrollPersonId.clear();
        faceId.clear();
        if ( ! faceBase->containsFace(faceKey))
            return enrollError(tr("FaceKey %1 does not exist").arg(faceKey));
    }
    else
    {
        if (faceId.isEmpty())
            return enrollError(tr("FaceId or FaceKey must be specified"));

        enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
        enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
        if (enrollPersonKey)
        {
            enrollPersonId = faceBase->personId(enrollPersonKey);
            if (enrollPersonId.isEmpty())
                return enrollError(tr("PersonKey %1 does not exist").arg(enrollPersonKey));
        }
        else
        {
            if ( ! enrollPersonId.isEmpty())
            {
                enrollPersonKey = faceBase->personKey(enrollPersonId);
                if ( ! enrollPersonKey)
                    return enrollError(tr("PersonId %1 does not exist").arg(enrollPersonId));
            }
        }
        faceKey = faceBase->faceKey(faceId, enrollPersonKey);
        if ( ! faceKey)
            return enrollError(enrollPersonKey
                               ? tr("FaceId %1 does not exist for %2").arg(faceId).arg(enrollPersonId)
                               : tr("FaceId %1 does not exist unidentfied").arg(faceId));
    }

    return enrollError("Reenroll is not yet supported");
} // doReenroll()
