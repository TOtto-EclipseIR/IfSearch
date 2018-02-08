#-------------------------------------------------
#
# Project created by QtCreator 2011-12-23T14:46:09
#
#-------------------------------------------------

QT       -= gui

TARGET = eirBase
TEMPLATE = lib

DEFINES += EIRBASE_LIBRARY

include(../libs.pri)

win32:RC_FILE = eirBase.rc

SOURCES += eirBase.cpp \
    VersionInfo.cpp \
    BaseIdBehavior.cpp \
    AbstractIdString.cpp \
    Enumeration.cpp \
    KeyTypes.cpp \
    KeyClassBehavior.cpp \
    KeyClass.cpp \
    UniversalKey.cpp \
    Severity.cpp \
    SeverityMask.cpp \
    EightCC.cpp \
    Library.cpp \
    BaseLog.cpp

HEADERS += eirBase.h\
        eirBase_global.h \
    VersionInfo.h \
    version.h \
    QProperty.h \
    Property.h \
    StaticInitialize.h \
    FixedPoint.h \
    BaseIdBehavior.h \
    AbstractIdString.h \
    Named.h \
    Enumeration.h \
    KeyTemplate.h \
    KeyTypes.h \
    KeyClassBehavior.h \
    KeyClass.h \
    UniversalKey.h \
    Named.h \
    Severity.h \
    ../../../common/version.h \
    Singleton.h \
    SeverityMask.h \
    SeverityArray.h \
    EightCC.h \
    DataProperty.h \
    Library.h \
    BaseLog.h

OTHER_FILES += \
    eirBase.rc \
    buildMe.pri \
    Severity.odt \
    ../../../common/WinRes.rc




































