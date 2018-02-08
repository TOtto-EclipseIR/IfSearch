CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../dbg
        LIBS += -L../../dbg
}
else {
        DESTDIR = ../../bin
        LIBS += -L../../bin
}
