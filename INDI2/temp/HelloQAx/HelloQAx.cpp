#include "HelloQAx.h"

#include <ActiveQt/QAxObject>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ax_obj(new QAxObject(this))
    , doc_browser(new QTextBrowser)
    , load_action(new QAction(tr("Load Control"), this))
{
    QToolBar * tb = new QToolBar(tr("Main"), this);
    tb->addAction(load_action);
    setCentralWidget(doc_browser);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::doLoad(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Control File",
                                                    QString(),
                                                    tr("Control Binaries (*.dll);;"
                                                       "All Files (*.*)"));
    if (fileName.isNull())  return;

    ax_obj->setControl(fileName);
    QString ctl_doc = ax_obj->generateDocumentation();
    doc_browser->setText(ctl_doc);
}
