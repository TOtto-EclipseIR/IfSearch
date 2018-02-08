#-------------------------------------------------
#
# Project created by QtCreator 2012-01-14T15:08:48
#
#-------------------------------------------------

QT       += network svg xml testlib

TARGET = eirCoreTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../tests.pri)
LIBS += -leirCore

SOURCES += eirCoreTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    ../../testData/VariableSettingsTest.ini \
    ../../testData/VariableSettingsTest2.ini
