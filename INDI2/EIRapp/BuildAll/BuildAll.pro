#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T20:23:54
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BuildAll
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../app.pri)
include(../../common/use/eirBase.pri)
include(../../common/use/eirType.pri)

LIBS *= -leirBase

SOURCES += main.cpp
