# common/libtests.pri - qmake definitions for library tests

INCLUDEPATH += ..
LIBS += -lEIRcore -lEIRxml -lEIRobjd

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../dbg
        LIBS += -L../dbg
}
else {
        DESTDIR = ../bin
        LIBS += -L../bin
}
linux-g++: LIBS+=-L/usr/lib64
