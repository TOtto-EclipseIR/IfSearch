DEFINES *= DETECTOR_SCALE=4.0
DEFINES *= USE_OCV4
CONFIG += debug_and_release

win32-g++: INCLUDEPATH *= ../../../../lang/opencv4/build/install/include
#win32-g++: LIBS        *= ../../../../lang/opencv4/build/install/x64/mingw/lib
linux-g++: INCLUDEPATH *= /usr/local/include
#linux-g++: LIBS *= -L/usr/local/lib
linux-g++: LIBS += /usr/local/lib/libopencv_core.so.4.5.5
linux-g++: LIBS += /usr/local/lib/libopencv_imgproc.so.4.5.5
linux-g++: LIBS += /usr/local/lib/libopencv_objdetect.so.4.5.5
win32-g++: LIBS += ../../../../lang/opencv4/build/install/x64/mingw/lib/libopencv_core455.dll.a
win32-g++: LIBS += ../../../../lang/opencv4/build/install/x64/mingw/lib/libopencv_imgproc455.dll.a
win32-g++: LIBS += ../../../../lang/opencv4/build/install/x64/mingw/lib/libopencv_objdetect455.dll.a


