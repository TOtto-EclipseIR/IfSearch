#-------------------------------------------------
#
# Project created by QtCreator 2014-03-08T20:17:59
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eirCfg
TEMPLATE = lib

DEFINES += EIRCFG_LIBRARY

win32:RC_FILE = eirCfg.rc

include(../libs.pri)
LIBS *= -leirBase -leirType -l eirLog -leirVar

SOURCES += eirCfg.cpp \
    Configuration.cpp \
    ConfigurationSettings.cpp \
    ConfigurationStack.cpp

HEADERS += eirCfg.h\
        eircfg_global.h \
    Configuration.h \
    ConfigurationSettings.h \
    ConfigurationStack.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
