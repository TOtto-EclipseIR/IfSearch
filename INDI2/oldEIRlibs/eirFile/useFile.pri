QMAKE_EXTRA_TARGETS += eirFile
PRE_TARGETDEPS += $$DESTDIR/libeirFile.a
eirFile.target = $$DESTDIR/libeirFile.a
LIBS += -leirFile
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirFile.commands = cd ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirFile.depends = ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirFile_debug.target = ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirFile_debug.commands = qmake.exe ../../EIRlibs/eirFile/eirFile.pro -r -spec win32-g++ "CONFIG=debug"
        QMAKE_EXTRA_TARGETS += eirFile_debug
}
else {
        eirFile.commands = cd ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirFile.depends = ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirFile_Release.target = ../../EIRlibs/eirFile-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirFile_Release.commands = qmake.exe ../../EIRlibs/eirFile/eirFile.pro -r -spec win32-g++ "CONFIG=Release"
        QMAKE_EXTRA_TARGETS += eirFile_Release
}
