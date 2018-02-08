#-------------------------------------------------
#
# Project created by QtCreator 2012-01-27T12:47:37
#
#-------------------------------------------------

QT       += network svg xml

TARGET = eirExe
TEMPLATE = lib

DEFINES += EIREXE_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore

SOURCES += eirExe.cpp \
    LogItem.cpp \
    LogDevice.cpp \
    LogFunction.cpp \
    Logger.cpp \
    LogFilter.cpp \
    LogDeviceText.cpp \
    LogDeviceNull.cpp \
    LogOptions.cpp \
    Console.cpp \
    ExeSupport.cpp \
    GuiApplication.cpp \
    Results.cpp \
    ResultCode.cpp \
    ResultId.cpp \
    ResultInfo.cpp \
    Result.cpp \
    ResultRegistry.cpp \
    OptionSet.cpp

HEADERS += eirExe.h\
        eirExe_global.h \
    LogItem.h \
    LogDevice.h \
    LogFunction.h \
    Log.h \
    Logger.h \
    LogFilter.h \
    LogDeviceText.h \
    LogDeviceNull.h \
    LogOptions.h \
    Console.h \
    ExeSupport.h \
    GuiApplication.h \
    Results.h \
    ResultCode.h \
    ResultId.h \
    ResultInfo.h \
    Result.h \
    ResultRegistry.h \
    OptionSet.h


