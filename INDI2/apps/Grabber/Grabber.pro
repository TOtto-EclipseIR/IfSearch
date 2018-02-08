#-------------------------------------------------
#
# Project created by QtCreator 2012-05-16T15:38:14
#
#-------------------------------------------------

QT       += core gui svg

TARGET = Grabber
TEMPLATE = app

include(../apps.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe
LIBS *= -leirGUI -leirFile -leirImage -leirFrameSource

win32:RC_FILE = Grabber.rc

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h \
    version.h \
    ../../../common/version.h

OTHER_FILES += \
    buildMe.pri \
    Grabber.rc \
    GrabberIR.ico \
    ../../../common/WinRes.rc
