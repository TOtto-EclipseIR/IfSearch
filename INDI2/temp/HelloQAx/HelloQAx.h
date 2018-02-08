#ifndef HELLOQAX_H
#define HELLOQAX_H

#include <QtGui/QMainWindow>

class QAction;
class QAxObject;
class QTextBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget * parent=0);
    ~MainWindow();

private slots:
    void doLoad(void);

private:
    QAxObject * ax_obj;
    QTextBrowser * doc_browser;
    QAction * load_action;
};

#endif // HELLOQAX_H
