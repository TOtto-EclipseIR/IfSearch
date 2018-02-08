DEFINES += DETECTOR_SCALE=3.0 USE_OCV2
CONFIG += debug_and_release
CONFIG(debug, debug|release)
{
    win32-g++:	INCLUDEPATH += /Lang/OpenCV2.0/include/opencv
    linux-g++:	INCLUDEPATH += /usr/local/include/opencv
    linux-g++:  LIBS += -L/usr/local/lib
    win32-g++:	LIBS += -L/Lang/OpenCV2.0/bin
    linux-g++:	LIBS += -lcxcore -lcv
    win32-g++:	LIBS += -lcxcore200 -lcv200
}
else {
    win32-g++:	INCLUDEPATH += /Lang/OpenCV2.0/include/opencv
    linux-g++:	INCLUDEPATH += /usr/local/include/opencv
    linux-g++:  LIBS += -L/usr/local/lib
    win32-g++:	LIBS += -L/Lang/OpenCV2.0/bin
    linux-g++:	LIBS += -lcxcore -lcv
    win32-g++:	LIBS += -lcxcore200 -lcv200
}
