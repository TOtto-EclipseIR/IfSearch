#-------------------------------------------------
#
# Project created by QtCreator 2012-01-16T14:40:24
#
#-------------------------------------------------

QT       += network svg xml testlib

TARGET = eirBaseTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../tests.pri)
LIBS *= -leirBase


SOURCES += eirBaseTest.cpp \
    testEightCC.cpp \
    testEnumeration.cpp \
    testKey.cpp \
    testSeverity.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"



