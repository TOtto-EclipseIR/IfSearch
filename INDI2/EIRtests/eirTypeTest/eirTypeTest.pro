#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T13:44:36
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = eirTypeTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../tests.pri)
LIBS *= -leirBase -leirType

SOURCES += eirTypeTest.cpp \
    TestReadable.cpp \
    TestHexdump.cpp \
    TestParsing.cpp \
    TestDomElement.cpp \
    TestSortable.cpp \
    TestCircularQueue.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
