#include "MainWindow.h"
#include "LivePage.h"
#include "IdPane.h"

#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>

#include <eirExe/Log.h>
#include <eirFile/QQDir.h>
#include <eirGUI/CancelAction.h>
#include <eirGUI/CheckAction.h>
#include <eirGUI/Gallery.h>
#include <eirGUI/GalleryItem.h>
#include <eirGUI/ImageLabel.h>
#include <eirGUI/QQIcon.h>
#include <eirSDK1/ImageDirSink.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>
#include <eirTypes/QQSound.h>

void LivePage::enterManualSearch(void)
{
    FUNCTION();
    STATE("Manual Search");
    emit stateName("Manual Search");

    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(IdPane, idPane());

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);

    search_result = SearchCommand();
    QTimer::singleShot(10, gatherSdk(), SLOT(pause()));
    qDebug() << "LivePage::enterManualSearch() 2";
    top_gallery->moveSelection(false);
    bot_gallery->clear();
    bot_gallery->setSelectionMode(Gallery::SelectMultiple);
    qDebug() << "LivePage::enterManualSearch() 3";
    search_result.setTimeoutMsec(5000);
    GalleryItemList items = top_gallery->selectedList();
    foreach (GalleryItem * gi, items)
    {
        GVIPOINTER(GalleryItem, gi);
        search_result.addInputImage(gi->image(), gi->id());
    }
    idPane()->message("Searching");
    CONNECT(searchSdk(), SIGNAL(searchResult(SdkCommand)),
            this, SLOT(searchResult(SdkCommand)));
    searchSdk()->submit(search_result);
}

void LivePage::enterSearchResult(void)
{
    FUNCTION();
    STATE("Search Result");
    emit stateName("Search Result");

    OBJPOINTER(SdkClient, searchSdk());
    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, manualSearch_action);
    OBJPOINTER(QAction, enrollNew_action);

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);

    search_result.parseOutput();
    search_result.parseResult();
    DUMPVSET(search_result);
    DISCONNECT(searchSdk(), SIGNAL(searchResult(SdkCommand)),
            this, SLOT(searchResult(SdkCommand)));

    idPane()->reset();
    bot_gallery->clear();
    bot_gallery->setSelectionMode(Gallery::SelectSingle);
    int nPerson = search_result.resultCount();
    for (int rank = 1; rank <= nPerson; ++rank)
    {
        QImage img = search_result.resultFace(rank, 1);
        QString id = search_result.personId(rank);
        int confidence = search_result.personMatchConfidence(rank);
        GalleryItem * gi = bot_gallery->add(img, id);
        GVIPOINTER(GalleryItem, gi);
        gi->addDecoration(GalleryItem::Score, confidence);
        gi->addDecoration(GalleryItem::Title, id);
    }
    bot_gallery->showPosition(0);
    idPane()->message(QString("%1 results").arg(nPerson));

    manualSearch_action->setChecked(true);
    manualSearch_action->setEnabled(true);
    enrollNew_action->setChecked(false);
    enrollNew_action->setEnabled(top_gallery->selectedList().size());
    CONNECT(top_gallery, SIGNAL(hasSelected(bool)),
            enrollNew_action, SLOT(setEnabled(bool)));
}

void LivePage::leaveSearchResult(void)
{
    FUNCTION();
    STATE("-Search Result");

    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, enrollNew_action);
    OBJPOINTER(QAction, manualSearch_action);

    DISCONNECT(top_gallery, SIGNAL(hasSelected(bool)),
               enrollNew_action, SLOT(setEnabled(bool)));
    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    idPane()->reset();
}

void LivePage::enterPersonResult(void)
{
    FUNCTION();
    STATE("Person Result");
    emit stateName("Person Result");

    OBJPOINTER(ImageLabel, bot_image);
    OBJPOINTER(Gallery, bot_gallery);

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    idPane()->reset();

    GalleryItem * gi = bot_gallery->selectedItem();
    if ( ! gi)
    {
        emit endPersonResult();
        return;
    }

    GVIPOINTER(GalleryItem, gi);
    TRACE("PersonResult selected id = %1", gi->id());
    QImage image(gi->image());
    QString personId(gi->id());
    resultPersonId = personId;
    idPane()->setId(personId);
    bot_image->set(image);
    bot_gallery->clear();
    bot_gallery->setSelectionMode(Gallery::SelectSingle);
    //search_result.dump();

    int nPerson = search_result.resultCount();
    for (int rank = 1; rank <= nPerson; ++rank)
    {
        if (personId == search_result.personId(rank))
        {
            personResult_rank = rank;
            int n = search_result.numFaces(rank);
            int fx;
            for (fx = 1; fx <= n; ++fx);
            {
                QImage img(search_result.resultFace(rank, fx));
                GalleryItem * gi = bot_gallery->add(img, QString::number(fx));
                if (1 == fx)
                {
                    GVIPOINTER(GalleryItem, gi);
                    bot_gallery->select(gi);
                }
            }
            break;
        }
    }

    bot_gallery->showPosition(0);
    addEnroll_action->setChecked(false);
    addEnroll_action->setEnabled(top_gallery->selectedList().size());
    CONNECT(top_gallery, SIGNAL(hasSelected(bool)),
            addEnroll_action, SLOT(setEnabled(bool)));

    SdkCommand command = searchSdk()->newCommand("Retrieve", "Retrieve");
    command.set(VariableId("Settings/Retrieve/PersonId"), personId);
    CONNECT(searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(retrieveResult(SdkCommand)));
    CONNECT(this, SIGNAL(gotRetrieveResult()),
            this, SLOT(displayRetrieveResult()));
    TRACE("PersonResult Retrieve/Retrieve for %1", personId);
    searchSdk()->submit(command);
    CONNECT(bot_gallery, SIGNAL(selected(GalleryItem*)),
            this, SLOT(personImageSelected(GalleryItem*)));
    CONNECT(bot_image, SIGNAL(clicked()),
            this, SLOT(personResultClicked()));
}

void LivePage::leavePersonResult(void)
{
    FUNCTION();
    STATE("-Person Result");
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(ImageLabel, bot_image);

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    idPane()->reset();

    bot_image->clear();
    retrieve_result = RetrieveCommand();
    personResult_rank = 0;
    DISCONNECT(bot_gallery, SIGNAL(selected(GalleryItem*)),
               this, SLOT(personImageSelected(GalleryItem*)));
    DISCONNECT(bot_image, SIGNAL(clicked()),
               this, SLOT(personResultClicked()));
}


void LivePage::manualSearch(bool checked)
{
    FUNCTION(checked);
    if (checked)
        emit doManualSearch();
    else
        emit endManualSearch();
}

void LivePage::searchResult(SdkCommand result)
{
    FUNCTION(result.key());
    if (result.isValid())
    {
        search_result = result;
        TRACE("emit gotSearchResult()");
        emit gotSearchResult();
    }
    else
    {
        TRACE("searchResult error: %1", result.getReason());
        idPane()->error("Search Error: " + result.getReason());
        idPane()->resetIn(5000);
        TRACE("emit searchResultError()");
        emit searchResultError();
    }
}

void LivePage::retrieveResult(SdkCommand result)
{
    FUNCTION(result.key());
    if (result.isValid())
    {
        retrieve_result = result;
        TRACE("emit gotRetrieveResult()");
        emit gotRetrieveResult();
    }
    else
    {
        TRACE("retrieveResult error: %1", result.getReason());
        idPane()->error("Retrive Error: " + result.getReason());
        idPane()->resetIn(5000);
        TRACE("emit retrieveError()");
        emit retrieveError();
    }
}

void LivePage::displayRetrieveResult(void)
{
    FUNCTION();
    DISCONNECT(searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(retrieveResult(SdkCommand)));
    DISCONNECT(this, SIGNAL(gotRetrieveResult()),
            this, SLOT(displayRetrieveResult()));

    retrieve_result.parseResult();
    retrieve_result.parseOutput();
    DUMPVSET(retrieve_result);
    QString token(retrieve_result.value("Settings/Description").toString());
    int n = retrieve_result.resultCount();
    for (int fx = 1; fx <= n; ++fx)
    {
        int faceKey = retrieve_result.faceKey(fx);
        QImage img(retrieve_result.resultFace(fx));
        QString faceId(QString("FK%1").arg(faceKey, 6, 10, QChar('0')));
        if ( ! bot_gallery->contains(faceId))
            bot_gallery->add(img, faceId);
    }
    idPane()->progress(1000,
                       0,
                       search_result.personMatchConfidence(personResult_rank));
    idPane()->setToken(token);
    bot_gallery->showPosition(0);
}

void LivePage::personImageSelected(GalleryItem * item)
{
    OBJPOINTER(ImageLabel, bot_image);
    GVIPOINTER(GalleryItem, item);
    FUNCTION(item->id());
    bot_image->set(item->image());
}

void LivePage::personResultClicked(void)
{
    FUNCTION();
    TRACE("emit retrieveError()");
    emit endPersonResult();
}

