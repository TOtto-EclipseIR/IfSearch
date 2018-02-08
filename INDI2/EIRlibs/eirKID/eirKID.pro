#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T22:24:54
#
#-------------------------------------------------

QT       -= gui

TARGET = eirKID
TEMPLATE = lib

DEFINES += EIRKID_LIBRARY

win32:RC_FILE = eirKID.rc

include(../libs.pri)

LIBS *= -leirBase -leirType

SOURCES += eirKID.cpp \
    BaseIdString.cpp \
    BasicId.cpp \
    UniversalKey.cpp \
    BasicIdBehavior.cpp \
    KeyClass.cpp \
    ClassId.cpp \
    KeySegment.cpp \
    KeyInfo.cpp \
    KeyContext.cpp \
    BaseIdBehavior.cpp \
    KeyBuilder.cpp \
    KeyRegistry.cpp \
    KeySegmentList.cpp

HEADERS += eirKID.h\
        eirKID_global.h \
    BaseIdBehavior.h \
    BaseIdString.h \
    BasicId.h \
    UniversalKey.h \
    KeyClass.h \
    ClassId.h \
    KeySegment.h \
    KeyInfo.h \
    KeyContext.h \
    KeyBuilder.h \
    KeyRegistry.h \
    KeySegmentList.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE552E675
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirKID.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    eirKID.rc

