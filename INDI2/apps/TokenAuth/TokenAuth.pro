#-------------------------------------------------
#
# Project created by QtCreator 2011-10-28T20:40:29
#
#-------------------------------------------------

QT       += core gui svg

TARGET = TokenAuth
TEMPLATE = app

include(../apps.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe -leirFile -leirGUI -leirSDK1

win32:RC_FILE = TokenAuth.rc

SOURCES += main.cpp\
        MainWindow.cpp \
    StateMachine.cpp \
    IdPane.cpp \
    LivePage.cpp \
    MaintenancePage.cpp \
    LivePage-Authenticate.cpp \
    MainWindow-Initialize.cpp \
    LivePage-Search.cpp \
    LivePage-Enroll.cpp

HEADERS  += MainWindow.h \
    StateMachine.h \
    IdPane.h \
    LivePage.h \
    MaintenancePage.h \
    ../../../common/version.h \
    version.h

RESOURCES += \
    resource/TokenAuth.qrc

OTHER_FILES += \
    buildMe.pri \
    ../../../common/WinRes.rc \
    TokenAuth.rc \
    buildMe.pri












