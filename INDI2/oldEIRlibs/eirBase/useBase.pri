QMAKE_EXTRA_TARGETS += eirBase
PRE_TARGETDEPS += $$DESTDIR/libeirBase.a
eirBase.target = $$DESTDIR/libeirBase.a
LIBS += -leirBase
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirBase.commands = cd ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirBase.depends = ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirBase_debug.target = ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
        eirBase_debug.commands = qmake.exe ../../EIRlibs/eirBase/eirBase.pro -r -spec win32-g++ "CONFIG=debug"
        QMAKE_EXTRA_TARGETS += eirBase_debug
}
else {
        eirBase.commands = cd ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirBase.depends = ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirBase_release.target = ../../EIRlibs/eirBase-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
        eirBase_release.commands = qmake.exe ../../EIRlibs/eirBase/eirBase.pro -r -spec win32-g++ "CONFIG+=release"
        QMAKE_EXTRA_TARGETS += eirBase_release
}



