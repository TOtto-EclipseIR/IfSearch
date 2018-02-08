#-------------------------------------------------
#
# Project created by QtCreator 2011-09-05T20:30:05
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = eirNetwork
TEMPLATE = lib

DEFINES += EIRNETWORK_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe

SOURCES += eirNetwork.cpp \
    HttpManager.cpp \
    HttpRequest.cpp

HEADERS += eirNetwork.h\
        eirNetwork_global.h \
    HttpManager.h \
    HttpRequest.h

OTHER_FILES += \
    buildMe.pri
