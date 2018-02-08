INCLUDEPATH *= ../../EIRlibs
INCLUDEPATH *= ../../INDIlibs
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../dbg/plugins
        LIBS *= -L../../dbg
}
else {
        DESTDIR = ../../bin/plugins
        LIBS *= -L../../bin
}
