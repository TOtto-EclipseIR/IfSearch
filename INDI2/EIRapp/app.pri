INCLUDEPATH *= ../../EIRlibs
INCLUDEPATH *= ../../INDIlibs
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../../common/dbg2
        LIBS += -L../../../common/dbg2
}
else {
        DESTDIR = ../../../common/bin2
        LIBS += -L../../../common/bin2
}
