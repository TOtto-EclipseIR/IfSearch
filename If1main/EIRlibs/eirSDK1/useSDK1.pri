QMAKE_EXTRA_TARGETS += eirSDK1
PRE_TARGETDEPS += $$DESTDIR/libeirSDK1.a
eirSDK1.target = $$DESTDIR/libeirSDK1.a
LIBS += -leirSDK1
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        eirSDK1.commands = cd ../../EIRlibs/eirSDK1-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug && mingw32-make
        eirSDK1.depends = ../../EIRlibs/eirSDK1-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug/Makefile
}
else {
        eirSDK1.commands = cd ../../EIRlibs/eirSDK1-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release && mingw32-make
        eirSDK1.depends = ../../EIRlibs/eirSDK1-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Release/Makefile
}
