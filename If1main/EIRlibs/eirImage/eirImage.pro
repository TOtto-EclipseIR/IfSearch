#-------------------------------------------------
#
# Project created by QtCreator 2012-05-16T15:34:35
#
#-------------------------------------------------

QT       += network svg xml

TARGET = eirImage
TEMPLATE = lib

DEFINES += EIRIMAGE_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore
LIBS *= -leirExe -leirGUI

SOURCES += eirImage.cpp \
    ImageEntity.cpp \
    ImageClass.cpp \
    MessageImageGenerator.cpp \
    MessageImageConfiguration.cpp

HEADERS += eirImage.h\
        eirImage_global.h \
    ImageEntity.h \
    ImageClass.h \
    MessageImageGenerator.h \
    MessageImageConfiguration.h

OTHER_FILES += \
    buildMe.pri
