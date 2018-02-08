#-------------------------------------------------
#
# Project created by QtCreator 2011-10-31T17:55:29
#
#-------------------------------------------------

TARGET = eirTypes
TEMPLATE = lib

DEFINES += EIRTYPES_LIBRARY

include(../libs.pri)
LIBS += -leirBase

SOURCES += eirTypes.cpp \
    QQSizeF.cpp \
    MillisecondDelta.cpp \
    MillisecondTime.cpp \
    QQSound.cpp \
    QQRectF.cpp

HEADERS += eirTypes.h\
        eirTypes_global.h \
    QQSizeF.h \
    MillisecondDelta.h \
    MillisecondTime.h \
    QQSound.h \
    QQRectF.h

OTHER_FILES += \
    buildMe.pri
















