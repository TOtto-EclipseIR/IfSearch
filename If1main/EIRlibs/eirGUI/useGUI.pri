QMAKE_EXTRA_TARGETS += eirGUI
PRE_TARGETDEPS += $$DESTDIR/libeirGUI.a
eirGUI.target = $$DESTDIR/libeirGUI.a
LIBS += -leirGUI
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirGUI.commands = cd ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirGUI.depends = ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirGUI_debug.target = ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirGUI_debug.commands = qmake.exe ../../EIRlibs/eirGUI/eirGUI.pro -r -spec win32-g++ "CONFIG=debug"
        QMAKE_EXTRA_TARGETS += eirGUI_debug
}
else {
        eirGUI.commands = cd ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirGUI.depends = ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirGUI_release.target = ../../EIRlibs/eirGUI-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirGUI_release.commands = qmake.exe ../../EIRlibs/eirGUI/eirGUI.pro -r -spec win32-g++ "CONFIG=release"
        QMAKE_EXTRA_TARGETS += eirGUI_release
}
