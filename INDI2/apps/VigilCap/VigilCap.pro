#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T12:12:47
#
#-------------------------------------------------

QT       += core

#QT       -= gui

TARGET = VigilCap
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../apps.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe -leirFile
LIBS *= -lImVigil

SOURCES += main.cpp \
    VigilCap.cpp

HEADERS += \
    VigilCap.h \
    version.h

OTHER_FILES += \
    T2042.log
