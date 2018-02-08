#-------------------------------------------------
#
# Project created by QtCreator 2012-07-05T16:31:28
#
#-------------------------------------------------

QT       += network xml

TARGET = bhttp
TEMPLATE = lib

include(../framesources.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe
LIBS *= -leirFrameSource -leirImage -leirFile -leirNetwork


DEFINES += BHTTPPLUGIN_LIBRARY

SOURCES += bhttpPlugin.cpp \
    bhttpBehavior.cpp

HEADERS += bhttpPlugin.h\
        BhttpPlugin_global.h \
    bhttpBehavior.h
