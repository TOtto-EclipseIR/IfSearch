#-------------------------------------------------
#
# Project created by QtCreator 2012-01-13T13:25:50
#
#-------------------------------------------------

QT       += network svg xml

TARGET = eirIb2
TEMPLATE = lib

DEFINES += EIRIB2_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore

SOURCES += eirIb2.cpp \
    FrameStatistics.cpp \
    FramePerformance.cpp \
    FaceId.cpp

HEADERS += eirIb2.h\
        eirIb2_global.h \
    FrameStatistics.h \
    FramePerformance.h \
    FaceId.h

OTHER_FILES += \
    buildMe.pri
