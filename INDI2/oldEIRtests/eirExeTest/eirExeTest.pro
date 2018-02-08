#-------------------------------------------------
#
# Project created by QtCreator 2012-01-29T15:47:23
#
#-------------------------------------------------

QT       += network svg xml testlib

TARGET = eirExeTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../tests.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe

SOURCES += eirExeTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
