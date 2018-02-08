#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T18:44:16
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = eirBaseTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../tests.pri)
LIBS *= -leirWin -leirBase

SOURCES += eirBaseTest.cpp \
    testEnumeration.cpp \
    testSeverity.cpp \
    testVariant.cpp \
    testVariant.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    ../tests.pri
