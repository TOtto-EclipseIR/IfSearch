#-------------------------------------------------
#
# Project created by QtCreator 2012-01-13T13:26:22
#
#-------------------------------------------------

QT       += network svg xml

TARGET = eirIf2
TEMPLATE = lib

DEFINES += EIRIF2_LIBRARY

SOURCES += eirIf2.cpp

HEADERS += eirIf2.h\
        eirIf2_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2E1440D
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirIf2.dll
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
