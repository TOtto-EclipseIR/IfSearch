#-------------------------------------------------
#
# Project created by QtCreator 2012-05-16T15:35:26
#
#-------------------------------------------------

QT       += network svg xml

TARGET = eirFrameSource
TEMPLATE = lib

DEFINES += EIRFRAMESOURCE_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe
LIBS *= -leirImage -leirFile -leirNetwork

SOURCES += eirFrameSource.cpp \
    FrameSequence.cpp \
    FrameSourcePlugin.cpp \
    FrameSourceMachine.cpp \
    FrameSourceState.cpp \
    FrameSourceManager.cpp \
    HttpBehavior.cpp \
    StaticDirsBehavior.cpp \
    LiveDirBehavior.cpp \
    FrameSourceBehavior.cpp \
    FrameSourceInterfaceBase.cpp \
    FrameSourceDirect.cpp \
    FrameSourceResult.cpp \
    FrameSourcePlugin-common.cpp \
    FrameSourceStatusIndex.cpp \
    FrameSourceChannelInfo.cpp \
    FrameSourceEnumerationCache.cpp \
    FrameSourceEnumeration.cpp \
    FrameSourcePlugin-EnterExit.cpp \
    FrameSourcePlugin-Interface.cpp

HEADERS += eirFrameSource.h\
        eirFrameSource_global.h \
    FrameSequence.h \
    FrameSourcePlugin.h \
    FrameSourceMachine.h \
    FrameSourceState.h \
    FrameSourceBehavior.h \
    FrameSourceManager.h \
    HttpBehavior.h \
    StaticDirsBehavior.h \
    LiveDirBehavior.h \
    FrameSourceInterface.h \
    FrameSourceBehavior.h \
    FrameSourceInterfaceBase.h \
    FrameSourceDirect.h \
    FrameSourceResult.h \
    FrameSourcePlugin-common.h \
    FrameSourceStatusIndex.h \
    FrameSourceChannelInfo.h \
    FrameSourceEnumerationCache.h \
    FrameSourceEnumeration.h \
    ../../../common/version.h

OTHER_FILES += \
    buildMe.pri
