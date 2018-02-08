#-------------------------------------------------
#
# Project created by QtCreator 2012-01-01T18:24:09
#
#-------------------------------------------------

QT       += network svg xml

TARGET = If2Face
TEMPLATE = lib

DEFINES += IF2FACE_LIBRARY

include(../libs.pri)
#include(../eirTypes/useTypes.pri)
include(../eirCore/useCore.pri)

SOURCES += If2Face.cpp \
    FaceId.cpp

HEADERS += If2Face.h\
        If2Face_global.h \
    FaceId.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE488BF25
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = If2Face.dll
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


