#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T13:42:41
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eirType
TEMPLATE = lib

DEFINES += EIRTYPE_LIBRARY

win32:RC_FILE = eirType.rc

include(../libs.pri)
LIBS *= -leirBase

SOURCES += eirType.cpp \
    TypeBehavior.cpp \
    Readable.cpp \
    Hexdump.cpp \
    Parsable.cpp \
    DataType.cpp \
    Parsed.cpp \
    DomVariant.cpp \
    VariantDom.cpp \
    Sortable.cpp \
    MillisecondDelta.cpp \
    MillisecondTime.cpp \
    EightCC.cpp \
    QQVariant.cpp \
    IntBehavior.cpp \
    StringBehavior.cpp

HEADERS += eirType.h\
    eirtype_global.h \
    version.h \
    DataType.h \
    TypeBehavior.h \
    Readable.h \
    Hexdump.h \
    Parsable.h \
    Parsed.h \
    DomVariant.h \
    VariantDom.h \
    Matrix.h \
    SparseMatrix.h \
    Sortable.h \
    MillisecondDelta.h \
    MillisecondTime.h \
    EightCC.h \
    QQVariant.h \
    IntBehavior.h \
    StringBehavior.h \
    CircularQueue.h \
    FixedPoint.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    eirType.rc \
    ../../common/use/eirType.pri


