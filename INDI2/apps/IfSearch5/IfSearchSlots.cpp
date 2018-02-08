#include "IfSearch.h"

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
#include <Resolver.h>
#include <HeightGrid.h>
#include <SkinMatcher.h>
#include <CharacteristicColor.h>
#include <ClothesMatchProperties.h>
#include <ClothesMatcher.h>

#include "../FSBridge/FSDirectBridge.h"

void IfSearch::pulse(void)
{
    //FUNCTION();
    //DETAIL("%1 grabs in cache", imageCache.grabSize());
    if (optShutdown->toBool())
    {
        QTimer::singleShot(10, this, SLOT(quit()));
        return;
    }

    if ( ! imageCache.isGrabEmpty())
        QTimer::singleShot(10, this, SLOT(processGrab()));

    if ( ! pendingFaces.isEmpty())
        QTimer::singleShot(10, this, SLOT(processFace()));

    imageCache.cleanAll();
    if (camera && imageCache.isGrabEmpty() && ! paused)
    {
        int rs = optRestartSecs->toInt();
        int sec = camera->msecsSinceLastGrab() / 1000;
        DETAIL("%1 seconds since last grab", sec);
        if (rs && sec >= rs)
        {
            WARNING("Restarting Camera");
            camera->restart();
        }
    }

    QTimer::singleShot(500, this, SLOT(pulse()));
} // pulse()

void IfSearch::hotdirReady(void)
{
    FUNCTION();
    NULLPTR(hotdir);
    if ( ! paused)
    {
        hotdir->cacheFirst();
        QTimer::singleShot(10, this, SLOT(processGrab()));
    }
}

void IfSearch::hotdirEmpty(void)
{
    PROGRESS(">>>>>>Input HotDir Processing Complete");
#ifdef ENABLE_AVGFACE
    if (optAvgFaceEnable->toBool())
        avgFaceFinish();
#endif
}


void IfSearch::grabbed(QString ImageId)
{
    FUNCTION();
    QTimer::singleShot(10, this, SLOT(processGrab()));
} // grabbed()

void IfSearch::error(QString msg)
{
    ERRMSG(msg);
}

void IfSearch::warn(QString msg)
{
    WARNING(msg);
}

void IfSearch::error2(QString id, QString msg)
{
    ERRMSG("%1 error for %2", msg, id);
}

void IfSearch::logRollover(void)
{
    PROGRESS("Log Rollover initiated");
    Info::rollover();
    PROGRESS("Log Rollover complete");
    Info::flush();
}


void IfSearch::directorySet(QString profileName, QString absoluteDirName)
{
    DETAIL("%1 directory set to %2", profileName,
         absoluteDirName.isEmpty() ? "[None]" : absoluteDirName);
}

void IfSearch::fileWritten(QString absoluteFilePath)
{
    DETAIL("Wrote %1", absoluteFilePath);
}

void IfSearch::done(void)
{
    Info::flush();
    if (fpWriter)
        fpWriter->close();
    if ( ! optNoPrompt->toBool())
    {
        printf("\n===Press <ENTER> to exit...");
        getchar();
    }
    delete appSettings;
    QTimer::singleShot(0, qApp, SLOT(quit()));
} // done()

void IfSearch::changed(QString key)
{
    NULLPTR(appSettings);
    INFO(tr("PC=%3 %1 changed to %2", "status"), key,
         appSettings->valueOf(key), appSettings->pollCount());

    if (false)
        ;
    else if (0 == key.compare(optShutdown->keyName(), Qt::CaseInsensitive))
    {
        if (optShutdown->toBool())
        {
            QTimer::singleShot(100, qApp, SLOT(done()));
            return;
        }
    }
    else if (0 == key.compare(tr("Input/URL", "config"), Qt::CaseInsensitive))
    {
        appSettings->setValue("Output/FramesProcessed",
                              QString::number(FramesProcessed=0));
        appSettings->setValue("Output/NullFrames",
                              QString::number(NullFrames=0));
        appSettings->setValue("Output/FacesProcessed",
                              QString::number(FacesProcessed=0));
        if (camera)
            camera->stop();
        imageCache.flushGrab();
        if (camera)
        {
            delete camera;
            camera = 0;
        }
        if (hotdir)
        {
            hotdir->stop();
            delete hotdir;
            hotdir = 0;
        }
        appSettings->setValue("Input/NumFiles", "0");

        /*---- Setup Input Sources ----*/

        QUrl url(optInput->toString());
        if (url.isEmpty())
        {
            WARNING("Input/URL is empty");
            Info::flush();
        }
        else if ( ! url.isValid())
        {
            WARNING("Input/URL %1 is not valid", url.toString());
        }
        else if (0 == url.scheme().compare("dir", Qt::CaseInsensitive))
        {
            streamBase_mst.set(MillisecondTime::null());
            hotdir = new InputHotdir(this);
            NULLPTR(hotdir);
            hotdir->setObjectName("hotdir");
            hotdir->setCache(&imageCache);
            appSettings->objectProperties(hotdir, tr("Input", "config"),
                                          QStringList() << "SampleMsec" << "NewOnly"
                                          << "DeleteAfter" << "MoveAfter"
                                          << "BeginDateTime" << "EndDateTime"
                                          << "Skip" << "Jump" << "Loop" << "ImageId"
                                          << "MaxCache" << "OrderBy",
                                          Settings::Volatile);
            connect(hotdir, SIGNAL(ready()), this, SLOT(hotdirReady()));
            connect(hotdir, SIGNAL(empty()), this, SLOT(hotdirEmpty()));
            connect(hotdir, SIGNAL(grabCached(QString)), this, SLOT(grabbed(QString)));
            connect(hotdir, SIGNAL(grabError(QString)), this, SLOT(error(QString)));
            connect(hotdir, SIGNAL(grabWarn(QString)), this, SLOT(warn(QString)));

            QUrl url(optInput->toString());
            PROGRESS(tr("Starting %1", "progress message"), url);
            Info::flush();
            QString err = hotdir->start(url);
            if (err.isEmpty())
            {
                INFO(tr("File types supported: %1", "progress message"),
                     hotdir->filters().join(" "));
                INFO(tr("%1 files ready in %2", "progress message"),
                     hotdir->numPending(), hotdir->absolutePath());
                appSettings->setValue("Input/NumFiles",
                                      QString::number(hotdir->numPending()));
                changed(optPause->keyName());
                QTimer::singleShot(100, this, SLOT(pulse()));
            }
            else
                ERRMSG(tr("Error starting %2: %1", "error message"),
                      err, optInput->toString());
            Info::flush();
        }
        else if ("http" == url.scheme())
        {
            streamBase_mst.set(MillisecondTime::null());
            camera = new ImageSource(this);
            NULLPTR(camera);
            camera->setObjectName("source");
            camera->setCache(&imageCache);
            QStringList qsl;
            qsl << "SampleMsec" << "SourceId" << "BeginDateTime" << "EndDateTime";
            appSettings->objectProperties(camera, tr("Input", "config"), qsl, Settings::Volatile);
            qsl.clear();
            qsl << "MaxCache";
            appSettings->objectProperties(camera, tr("Input", "config"), qsl, Settings::Volatile | Settings::Advanced);
            connect(camera, SIGNAL(grabCached(QString)), this, SLOT(grabbed(QString)));
            connect(camera, SIGNAL(grabError(QString)), this, SLOT(error(QString)));
            connect(camera, SIGNAL(grabWarn(QString)), this, SLOT(warn(QString)));

            QString err = camera->selectNetcam(optInput->toString(),
                                               appSettings->value("Input/Format").toString());
            if ( ! err.isEmpty())
                ERRMSG(tr("Error starting %2: %1", "error message"), err, optInput->toString());
            else
            {
                camera->start();
                changed(optPause->keyName());
            }
        }
        else
        {
            WARNING("Unsupported Input/URL scheme: %1", url.scheme());
        }
        appSettings->dump(InfoSeverity::Info, "Input/");
    }
    else if (0 == key.compare(optPause->keyName(), Qt::CaseInsensitive))
    {
        if (optPause->toBool())
        {
            if (camera)
                camera->setPaused(true);
            if (ffdBusy)
            {
                pausePending = true;
                PROGRESS("---PAUSE PENDING---");
            }
            else
            {
                imageCache.flushGrab();
                pendingFaces.clear();
                paused = true;
            }
        }
        else
        {
            if (camera)
                camera->setPaused(false);
            paused = false;
        }
    }
    else if (0 == key.compare(tr("Enroll/Command", "config"), Qt::CaseInsensitive))
    {
        if ( ! commandMode.isEmpty())
        {
            ERRMSG("[ENROLL] Command mode busy in %1=%2", commandMode,
                  appSettings->value(commandMode+"/Command").toString());
            appSettings->setValue("Enroll/Command", QString());
            return;
        }
        else
            commandMode = "Enroll";
        if (0 == optEnrollCommand->toString().compare("Enroll", Qt::CaseInsensitive))
        {
            enrollTimeout = 10;
            QTimer::singleShot(10, this, SLOT(doEnroll()));
        }
        else if (0 == optEnrollCommand->toString().compare("Reload", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doReload()));
        else if (0 == optEnrollCommand->toString().compare("Delete", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doDelete()));
        else if (0 == optEnrollCommand->toString().compare("RemoveAll", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doRemoveAll()));
        else if (0 == optEnrollCommand->toString().compare("Remove", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doRemove()));
        else if (0 == optEnrollCommand->toString().compare("Rename", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doRename()));
        else if (0 == optEnrollCommand->toString().compare("Manual", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doManual()));
        else if (0 == optEnrollCommand->toString().compare("Reenroll", Qt::CaseInsensitive))
            QTimer::singleShot(10, this, SLOT(doReenroll()));
        else
        {
            if ( ! optEnrollCommand->toString().isEmpty())
                ERRMSG("[ENROLL] Bad Command %1", optEnrollCommand->toString());
            commandMode.clear();
        }
    }
    else if (0 == key.compare(tr("Search/Command", "config"), Qt::CaseInsensitive))
    {
        if ( ! commandMode.isEmpty())
        {
            ERRMSG("[SEARCH] Command mode busy in %1=%2", commandMode,
                  appSettings->value(commandMode+"/Command").toString());
            appSettings->setValue("Search/Command", QString());
            return;
        }
        else
            commandMode = "Search";
        writer->pumpAll();
        if (0 == optSearchCommand->toString().compare("Search", Qt::CaseInsensitive))
        {
            searchTimeout = 10;
            QTimer::singleShot(10, this, SLOT(doSearch()));
        }
        else if (0 == optSearchCommand->toString().compare("Verify", Qt::CaseInsensitive))
        {
            searchTimeout = 10;
            QTimer::singleShot(10, this, SLOT(doVerify()));
        }
        else if (0 == optSearchCommand->toString().compare("Authenticate", Qt::CaseInsensitive))
        {
            searchTimeout = 10;
            QTimer::singleShot(10, this, SLOT(doAuthenticate()));
        }
        else if (0 == optSearchCommand->toString().compare("Similarity", Qt::CaseInsensitive))
        {
            searchTimeout = 10;
            QTimer::singleShot(10, this, SLOT(doSimilarity()));
        }
        else
        {
            if ( ! optSearchCommand->toString().isEmpty())
                ERRMSG("[SEARCH] Bad Command %1", optSearchCommand->toString());
            commandMode.clear();
        }
    }
    else if (0 == key.compare(optRetrieveCommand->keyName(), Qt::CaseInsensitive))
    {
        if ( ! commandMode.isEmpty())
        {
            ERRMSG("[RETRIEVE] Command mode busy in %1=%2", commandMode,
                  appSettings->value(commandMode+"/Command").toString());
            appSettings->setValue("Retrieve/Command", QString());
            return;
        }
        else
            commandMode = "Retrieve";
        writer->pumpAll();
        if (0 == optRetrieveCommand->toString().compare("Retrieve", Qt::CaseInsensitive))
        {
            QTimer::singleShot(10, this, SLOT(doRetrieve()));
        }
        else if (0 == optRetrieveCommand->toString().compare("RetrieveAll", Qt::CaseInsensitive))
        {
            QTimer::singleShot(10, this, SLOT(doRetrieveAll()));
        }
        else if (0 == optRetrieveCommand->toString().compare("Status", Qt::CaseInsensitive))
        {
            QTimer::singleShot(10, this, SLOT(doStatus()));
        }
        else if (0 == optRetrieveCommand->toString().compare("PersonReport", Qt::CaseInsensitive))
        {
            QTimer::singleShot(10, this, SLOT(doPersonReport()));
        }
        else
        {
            if ( ! optRetrieveCommand->toString().isEmpty())
                ERRMSG("[RETRIEVE] Bad Command %1", optRetrieveCommand->toString());
            commandMode.clear();
        }
    }
    else if (0 == key.compare(optResolveMin->keyName(), Qt::CaseInsensitive))
        ;
    else if (0 == key.compare(optResolveMax->keyName(), Qt::CaseInsensitive))
        ;
    else if (0 == key.startsWith("Resolve/"), Qt::CaseInsensitive)
    {
        NULLPTR(resolver);
        QString item = key.split("/").last();
        resolver->setWeight(item, appSettings->value(key).toInt());
    }
    else if (0 == key.compare(optHeightEnable->keyName(), Qt::CaseInsensitive)
            || 0 == key.compare("Height/GridFile", Qt::CaseInsensitive))
    {
        NULLPTR(heightGrid);
        if (optHeightEnable->toBool())
        {
            QImage heightImage = heightGrid->initializeGrid();
            if (fwpHeight->isActive() && ! heightImage.isNull())
                fwpHeight->write(heightImage, "HeightGrid");
        }
    }

} // changed()
