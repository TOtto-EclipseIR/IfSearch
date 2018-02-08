#-------------------------------------------------
#
# Project created by QtCreator 2011-10-27T11:33:07
#
#-------------------------------------------------

QT       += svg

TARGET = eirGUI
TEMPLATE = lib

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe

DEFINES += EIRGUI_LIBRARY

SOURCES += eirGUI.cpp \
    GalleryView.cpp \
    GalleryItem.cpp \
    GalleryParameters.cpp \
    Gallery.cpp \
    GalleryScene.cpp \
    GalleryBorder.cpp \
    KeyGrabButton.cpp \
    ImageLabel.cpp \
    IOWidget.cpp \
    GalleryDecoration.cpp \
    TimeoutBar.cpp \
    QQIcon.cpp \
    CheckAction.cpp \
    CancelAction.cpp

HEADERS += eirGUI.h\
        eirGUI_global.h \
    GalleryView.h \
    GalleryItem.h \
    GalleryParameters.h \
    Gallery.h \
    GalleryScene.h \
    GalleryBorder.h \
    KeyGrabButton.h \
    ImageLabel.h \
    IOWidget.h \
    GalleryDecoration.h \
    TimeoutBar.h \
    QQIcon.h \
    CheckAction.h \
    CancelAction.h

RESOURCES += \
    Resource/eirGUI.qrc

OTHER_FILES += \
    buildMe.pri \
    Resource/showLast.png \
    Resource/showFirst.png \
    Resource/Resume.svg \
    Resource/prevStep.png \
    Resource/prevPage.png \
    Resource/Pause.svg \
    Resource/nextStep.png \
    Resource/nextPage.png \
    Resource/eIRonly.svg









