#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T18:16:38
#
#-------------------------------------------------

QT       -= gui

TARGET = eirBase
TEMPLATE = lib

DEFINES += EIRBASE_LIBRARY

win32:RC_FILE = eirBase.rc

include(../libs.pri)
LIBS *= -leirWin

SOURCES += eirBase.cpp \
    Module.cpp \
    VersionInfo.cpp \
    Enumeration.cpp \
    Severity.cpp \
    SeverityMask.cpp \
    BaseLog.cpp \
    MemoryMonitorItem.cpp \
    MemoryMonitor.cpp

HEADERS += eirBase.h\
        eirbase_global.h \
    Module.h \
    Property.h \
    version.h \
    VersionInfo.h \
    Enumeration.h \
    Named.h \
    Severity.h \
    SeverityArray.h \
    SeverityMask.h \
    BaseLog.h \
    ../../../common/version.h \
    EnumerationMask.h \
    DataProperty.h \
    MemoryMonitorItem.h \
    MemoryMonitor.h \
    Singleton.h \
    EnumerationArray.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    eirBase.rc \
    ../../common/use/eirBase.pri
