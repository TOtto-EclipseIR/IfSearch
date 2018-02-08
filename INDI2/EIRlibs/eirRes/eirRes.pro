#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T15:57:17
#
#-------------------------------------------------

QT       -= gui

TARGET = eirRes
TEMPLATE = lib

DEFINES += EIRRES_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirType -leirKID -leirLog

SOURCES += eirRes.cpp \
    Result.cpp \
    ResultCode.cpp \
    ResultId.cpp \
    ResultInfo.cpp \
    ResultRegistry.cpp \
    Results.cpp

HEADERS += eirRes.h\
        eirres_global.h \
    Result.h \
    ResultCode.h \
    ResultId.h \
    ResultInfo.h \
    ResultRegistry.h \
    Results.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
