#include "GuiApplication.h"

#include <QtGui/QApplication>

GuiApplication::GuiApplication(QWidget * parent)
    : QMainWindow(parent)
    , ExeSupport(qApp)
{
}


