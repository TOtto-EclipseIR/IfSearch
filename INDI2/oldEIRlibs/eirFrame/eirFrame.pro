#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T14:58:32
#
#-------------------------------------------------

QT       += network sql xml

TARGET = eirFrame
TEMPLATE = lib

DEFINES += EIRFRAME_LIBRARY

SOURCES += eirFrame.cpp \
    FramePerformance.cpp \
    FrameStatistics.cpp

HEADERS += eirFrame.h\
        eirFrame_global.h \
    FramePerformance.h \
    FrameStatistics.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE36340A5
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirFrame.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
