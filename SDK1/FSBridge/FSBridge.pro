#-------------------------------------------------
#
# Project created by QtCreator 2012-07-18T16:44:33
#
#-------------------------------------------------

QT       += network svg xml

QMAKE_CXXFLAGS += -fpermissive \
    -DQ_DECL_IMPORT= \
    -DQ_DECL_EXPORT=
QMAKE_LFLAGS += --enable-stdcall-fixup -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc

TARGET = FSBridge
TEMPLATE = lib

DEFINES += FSBRIDGE_LIBRARY

INCLUDEPATH *= ../../INDI2/EIRlibs
INCLUDEPATH *= ../../INDI2/INDIlibs
INCLUDEPATH *= ../IfSearch

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../common/dbg
        LIBS += -L../../common/dbg
}
else {
        DESTDIR = ../../common/bin
        LIBS += -L../../common/bin
}
INCLUDEPATH += .

LIBS *= -leirBase -leirTypes -leirCore -leirExe
LIBS *= -leirFrameSource -leirImage -leirFile -leirNetwork

SOURCES += FSBridge.cpp \
    FSDirectBridge.cpp \
    ../IfSearch/ImageCache.cpp

HEADERS += FSBridge.h\
        FSBridge_global.h \
    FSDirectBridge.h \
    ../IfSearch/ImageCache.h \
    ../IfSearch/iImageCache.h

