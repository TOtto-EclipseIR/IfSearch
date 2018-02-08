#-------------------------------------------------
#
# Project created by QtCreator 2011-10-10T17:53:50
#
#-------------------------------------------------

QT       += sql xml

QT       -= gui

TARGET = eirKeyId
TEMPLATE = lib

include(../libs.pri)

DEFINES += EIRKEYID_LIBRARY

SOURCES += eirKeyId.cpp \
    BaseIdBehavior.cpp \
    AbstractIdString.cpp

HEADERS += eirKeyId.h\
        eirKeyId_global.h \
    BaseIdBehavior.h \
    AbstractIdString.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3F52E70
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirKeyId.dll
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
