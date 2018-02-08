INCLUDEPATH *= ../../EIRlibs
INCLUDEPATH *= ../../INDIlibs
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../../common/dbg
        LIBS *= -L../../../common/dbg
}
else {
        DESTDIR = ../../../common/bin
        LIBS *= -L../../../common/bin
}
