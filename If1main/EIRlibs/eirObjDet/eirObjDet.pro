#-------------------------------------------------
#
# Project created by QtCreator 2011-10-25T17:10:49
#
#-------------------------------------------------

QT       += network xml

TARGET = eirObjDet
TEMPLATE = lib

DEFINES += EIROBJDET_LIBRARY

SOURCES += eirObjDet.cpp \
    ObjDetParameters.cpp

HEADERS += eirObjDet.h\
        eirObjDet_global.h \
    ObjDetParameters.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3C817F5
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirObjDet.dll
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
