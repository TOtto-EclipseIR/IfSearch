QMAKE_EXTRA_TARGETS += eirCore
PRE_TARGETDEPS += $$DESTDIR/libeirCore.a
eirCore.target = $$DESTDIR/libeirCore.a
LIBS += -leirCore
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirCore.commands = cd ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirCore.depends = ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirCore_debug.target = ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirCore_debug.commands = qmake.exe ../../EIRlibs/eirCore/eirCore.pro -r -spec win32-g++ "CONFIG=debug"
        QMAKE_EXTRA_TARGETS += eirCore_debug
}
else {
        eirCore.commands = cd ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirCore.depends = ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirCore_release.target = ../../EIRlibs/eirCore-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirCore_release.commands = qmake.exe ../../EIRlibs/eirCore/eirCore.pro -r -spec win32-g++ "CONFIG=release"
        QMAKE_EXTRA_TARGETS += eirCore_release
}
