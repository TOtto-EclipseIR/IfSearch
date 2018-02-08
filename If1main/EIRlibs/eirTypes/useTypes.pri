QMAKE_EXTRA_TARGETS += eirTypes
PRE_TARGETDEPS += $$DESTDIR/libeirTypes.a
eirTypes.target = $$DESTDIR/libeirTypes.a
LIBS += -leirTypes
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirTypes.commands = cd ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirTypes.depends = ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirTypes_debug.target = ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirTypes_debug.commands = qmake.exe ../../EIRlibs/eirTypes/eirTypes.pro -r -spec win32-g++ "CONFIG=debug"
        QMAKE_EXTRA_TARGETS += eirTypes_debug
}
else {
        eirTypes.commands = cd ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirTypes.depends = ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirTypes_release.target = ../../EIRlibs/eirTypes-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirTypes_release.commands = qmake.exe ../../EIRlibs/eirTypes/eirTypes.pro -r -spec win32-g++ "CONFIG=release"
        QMAKE_EXTRA_TARGETS += eirTypes_release
}
