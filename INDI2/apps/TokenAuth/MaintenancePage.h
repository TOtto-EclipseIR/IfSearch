#ifndef MAINTENANCEPAGE_H
#define MAINTENANCEPAGE_H

#include <QtGui/QWidget>

#include <QtCore/QDir>
class QAction;
class QToolBar;

#include <eirGUI/GalleryParameters.h>
#include <eirSDK1/SdkCommand.h>
class Gallery;
class SdkClient;

class MainWindow;
class IdPane;

class MaintenancePage : public QWidget
{
    Q_OBJECT
public:
    explicit MaintenancePage(MainWindow * parent);
    QAction * finishAction(void) const;

signals:
    void stateName(QString newText);

public slots:
    void setupUi(void);
    void clearTaib(void);
    void importTaib(void);
    void initiateRetriveAll(void);
    void displayRetrieveAll(SdkCommand result);
    void removeAllResult(SdkCommand result);
    void enterMaintenance(void);
    void leaveMaintenance(void);

private slots:
    void importNext(void);
    void importEnrollResult(SdkCommand result);

private:
    MainWindow * mainw(void) const;
    IdPane * idPane(void) const;
    SdkClient * searchSdk(void) const;

private:
    MainWindow * mw_ptr;
    QToolBar * maint_toolbar;
    QAction * clearTaib_action;
    QAction * importTaib_action;
    QAction * exportTaib_action;
    QAction * cancel_action;
    QAction * finish_action;
    GalleryParameters gallery_parms;
    Gallery * _gallery;
    QDir people_dir;
    QStringList personId_list;
    int imported_k;
};

#endif // MAINTENANCEPAGE_H
