#include "IfSearch.h"

#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceFace.h>
#include "EigenFaceStatus.h"
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
//#include <Trace.h>

void IfSearch::doEnroll(void)
{
    if (ffdBusy)
    {
        INFO("[ENROLL] ffdBusy");
        if (--enrollTimeout <= 0)
            return enrollError("Timeout");
        QTimer::singleShot(100, this, SLOT(doEnroll()));
        return;
    }

    PROGRESS("[ENROLL] Started");
    enrollResults.clear();
    appSettings->setValue("Enroll/Status", "Checking FaceBase");
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());
    if ( ! faceBase)
        return enrollError("No FaceBase");
    enrollPersonMode = appSettings->value("Enroll/PersonMode", true).toBool();

    QString sStatus(appSettings->value("Enroll/FaceStatus").toString());
    if (sStatus.isEmpty())
        enroll_faceStatus = EigenFaceStatus::New;
    else
    {
        enroll_faceStatus.set(sStatus);
        if ( ! enroll_faceStatus.isValid())
            return enrollError("Invalid FaceStatus");
    }

    enrollPersonKey = appSettings->value("Enroll/PersonKey", 0).toInt();
    enrollPersonId = appSettings->value("Enroll/PersonId", QString()).toString();
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
            if (enrollPersonId.isEmpty())
                return enrollError(tr("PersonId must be specified for new person"));

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
    else
    {
        enrollPersonKey = 0;
        enrollPersonId.clear();
    }

    QTimer::singleShot(10, this, SLOT(enrollScanDir()));
} // doEnroll()

void IfSearch::enrollScanDir(void)
{
    NULLPTR(appSettings); NULLPTR(writer);
    QStringList qsl;

    appSettings->setValue("Enroll/Status", "Scanning Directory");
    QString dirName = appSettings->value("Enroll/InputDir", "../Enroll").toString();
    if (dirName.isEmpty())
        return enrollError("Enroll/InputDir is blank");

    enrollDir = writer->baseDir();
    if ( ! enrollDir.cd(dirName))
        return enrollError(tr("Can't cd to %1").arg(dirName));
    enrollPending = enrollDir.entryList(QStringList() << "*.PNG" << "*.png", QDir::Files);
    if (enrollPending.isEmpty())
        return enrollError(tr("%1 is empty").arg(dirName));

    if (enrollPersonKey)
        enrollResults = tr("PersonId=%1 PersonKey=%2").arg(enrollPersonId).arg(enrollPersonKey);
    else
        enrollResults = tr("Unidentified enrollment");

    appSettings->setValue("Enroll/Status", "Reading Files");
    QTimer::singleShot(10, this, SLOT(enrollGrab()));
} // enrollScanDir()

void IfSearch::enrollGrab(void)
{
    if (enrollPending.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(enrollFinish()));
        return;
    }

    QString fileName = enrollPending.takeFirst();
    QFileInfo fi(enrollDir.absoluteFilePath(fileName));
    QString imageId = fi.completeBaseName();
    EigenFaceTemplate tpl;
    EigenFaceFace face;
    QImage normImage(fi.absoluteFilePath());
    if (normImage.isNull())
        return enrollError(tr("Null image in %1").arg(fileName));

    // TODO: Why does reading from normImage return empty template?
    Return rtn = tpl.read(fi.absoluteFilePath());
    if (rtn.isError())
        return enrollError(rtn.toString());

    int enrollFaceKey = faceBase->faceKey(imageId, enrollPersonKey);
    if (enrollFaceKey) // && ! optIgnoreExisting
    {
        enrollResults += resultsDelimiter + tr("%1 already enrolled").arg(imageId);
        QTimer::singleShot(10, this, SLOT(enrollGrab()));
        return;
    }

    face.setTemplate(tpl);
    if (EigenFaceStatus::New == enroll_faceStatus)
    {
        face.setStatus((enrollPersonMode
                        && (faceBase->personFaceKeys(enrollPersonKey).isEmpty()))
                ? EigenFaceStatus::First
                : EigenFaceStatus::Active);
    }
    else
        face.setStatus(enroll_faceStatus);
    face.setFaceKey(enrollFaceKey);
    face.setPersonKey(enrollPersonKey);
    face.setFaceId(imageId);
    face.setPersonId(enrollPersonId);
    rtn = faceBase->add(&face, normImage);
    RETURN(rtn);
    enrollResults += resultsDelimiter
            + tr("%1 enrolled as %2:%3 %4")
                .arg(imageId)
                .arg(face.personKey())
                .arg(face.faceKey())
                .arg(face.status().name());

    QTimer::singleShot(10, this, SLOT(enrollGrab()));
} // enrollGrab()

void IfSearch::enrollFinish(void)
{
    QString Description = appSettings->value("Enroll/SetDescription", QString()).toString();
    if ( ! Description.isEmpty())
        faceBase->setPersonDescription(enrollPersonKey, Description);
    Description = faceBase->personDescription(enrollPersonKey);
    appSettings->setValue("Enroll/Description", Description);

    PROGRESS("[ENROLL] Done - Results:");
    QStringList qsl = enrollResults.split(resultsDelimiter);
    foreach (QString s, qsl)
        INFO("\t%1", s);
    appSettings->setValue("Enroll/Results", enrollResults);
    appSettings->setValue("Enroll/Reason", QString());
    appSettings->setValue("Enroll/Status", "Done");
    appSettings->setValue("Enroll/Command", QString());
    Info::flush();
    commandMode.clear();
} // enrollFinish()

void IfSearch::enrollError(const QString & reason)
{
    ERRMSG("[ENROLL] Error: %1", reason);
    appSettings->setValue("Enroll/Results", QString());
    appSettings->setValue("Enroll/Reason", reason);
    appSettings->setValue("Enroll/Status", "Error");
    appSettings->setValue("Enroll/Command", QString());
    Info::flush();
    commandMode.clear();
} // enrollFinish()
