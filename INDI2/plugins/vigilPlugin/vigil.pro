#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T16:35:55
#
#-------------------------------------------------

QT       += network xml

TARGET = vigil
TEMPLATE = lib

include(../framesources.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe
LIBS *= -leirImage -leirFile -leirNetwork -leirFrameSource

win32:RC_FILE = vigil.rc

DEFINES += VIGIL_LIBRARY

SOURCES += VigilPlugin.cpp \
    VigilBehavior.cpp

HEADERS += VigilPlugin.h\
        vigil_global.h \
    VigilBehavior.h

OTHER_FILES += \
    vigil.rc
