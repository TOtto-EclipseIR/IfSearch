INCLUDEPATH *= ../../EIRlibs
INCLUDEPATH *= ../../INDIlibs
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../../common/dbg/framesource
        LIBS *= -L../../../common/dbg
}
else {
        DESTDIR = ../../../common/bin/framesource
        LIBS *= -L../../../common/bin
}
