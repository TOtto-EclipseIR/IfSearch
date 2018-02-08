#include "mainwindow.h"

#include <QtGui/QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QLabel * hello_label = new QLabel("Hello Qt");
    setCentralWidget(hello_label);
}

MainWindow::~MainWindow()
{
}
