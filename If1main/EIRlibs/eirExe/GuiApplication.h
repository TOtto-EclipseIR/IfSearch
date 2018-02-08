#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include <QtGui/QMainWindow>

#include "ExeSupport.h"

class EIREXESHARED_EXPORT GuiApplication : public QMainWindow
                                         , public ExeSupport
{
    Q_OBJECT

public:
    explicit GuiApplication(QWidget * parent=0);

signals:
    
public slots:
    
};

#endif // GUIAPPLICATION_H
