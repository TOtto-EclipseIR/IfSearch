#-------------------------------------------------
#
# Project created by QtCreator 2011-12-02T11:44:32
#
#-------------------------------------------------

QT       += network xml

TARGET = eirCollection
TEMPLATE = lib

DEFINES += EIRCOLLECTION_LIBRARY

include(../libs.pri)
LIBS += -leirTypes

SOURCES += eirCollection.cpp

HEADERS += eirCollection.h\
        eirCollection_global.h \
    Matrix.h
