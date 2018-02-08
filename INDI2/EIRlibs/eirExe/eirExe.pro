#-------------------------------------------------
#
# Project created by QtCreator 2014-03-08T22:36:19
#
#-------------------------------------------------

QT       += svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = eirExe
TEMPLATE = lib

DEFINES += EIREXE_LIBRARY

win32:RC_FILE = eirExe.rc

include(../libs.pri)
LIBS *= -leirBase -leirType -leirLog -leirVar -leirCfg

SOURCES += eirExe.cpp \
    SystemConfiguration.cpp

HEADERS += eirExe.h\
        eirexe_global.h \
    SystemConfiguration.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
