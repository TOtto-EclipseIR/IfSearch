#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T18:49:27
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

include(../../eirTests/tests.pri)

TARGET = tst_TestKIDTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS *= -leirBase -leirKID

SOURCES += tst_TestKIDTest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
