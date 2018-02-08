#-------------------------------------------------
#
# Project created by QtCreator 2012-02-29T21:44:29
#
#-------------------------------------------------

QT       += network svg xml

CONFIG	+= qaxcontainer
#TYPELIBS = $$system(dumpcpp -getfile {5F19955C-C89B-460A-AB15-FA0CED5244C1})

TARGET = ImVigil
TEMPLATE = lib

DEFINES += IMVIGIL_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe

SOURCES += ImVigil.cpp \
    VigilSDK.cpp \
    VigilDvr.cpp \
    VigilResult.cpp \
    VigilCameraInfo.cpp \
    VigilCameraInfoList.cpp \
    VigilLiveFeed.cpp \
    VigilPlayback.cpp

HEADERS += ImVigil.h\
        ImVigil_global.h \
    VigilSDK.h \
    VigilDvr.h \
    VigilResult.h \
    VigilCameraInfo.h \
    VigilCameraInfoList.h \
    VigilLiveFeed.h \
    VigilPlayback.h

OTHER_FILES += \
    ../libs.pri
