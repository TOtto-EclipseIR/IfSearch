#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T13:58:44
#
#-------------------------------------------------

QT       -= gui

TARGET = eirLog
TEMPLATE = lib

DEFINES += EIRLOG_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirType

SOURCES += eirLog.cpp \
    Logger.cpp \
    LogItem.cpp \
    LogOptions.cpp \
    BasicLogOutput.cpp \
    LogFilterSet.cpp \
    LogFilter.cpp \
    LogFunction.cpp

HEADERS += eirLog.h\
        eirlog_global.h \
    Logger.h \
    Log.h \
    LogItem.h \
    LogOptions.h \
    BasicLogOutput.h \
    LogFilterSet.h \
    LogFilter.h \
    LogFunction.h \
    version.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
