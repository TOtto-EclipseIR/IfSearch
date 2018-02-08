#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T17:43:12
#
#-------------------------------------------------

QT       -= gui

TARGET = eirWin
TEMPLATE = lib

DEFINES += EIRWIN_LIBRARY
win32:DEFINES *= PSAPI_VERSION=1
#win32:LIBS += C:\lang\WindowsKits\8.1\Lib\winv6.3\um\x86\Psapi.lib
win32:LIBS += C:\lang\WindowsSDK\8.1\Lib\winv6.3\um\x86\Psapi.lib

include(../libs.pri)

SOURCES += eirWin.cpp \
    ProcessMemory.cpp \
    PerformanceInfo.cpp \
    WindowsError.cpp \
    SystemInfo.cpp

HEADERS += eirWin.h\
        eirwin_global.h \
    ProcessMemory.h \
    PerformanceInfo.h \
    WindowsError.h \
    SystemInfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    ../../common/use/eirWin.pri
