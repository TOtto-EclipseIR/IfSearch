#-------------------------------------------------
#
# Project created by QtCreator 2014-02-23T18:36:52
#
#-------------------------------------------------

QT       -= gui

TARGET = eirWork
TEMPLATE = lib

DEFINES += EIRWORK_LIBRARY

include(../libs.pri)

SOURCES += eirWork.cpp \
    AbstractWorker.cpp \
    WorkerObject.cpp \
    WorkerThread.cpp \
    WorkerRunnable.cpp

HEADERS += eirWork.h\
        eirWork_global.h \
    AbstractWorker.h \
    WorkerObject.h \
    WorkerThread.h \
    WorkerRunnable.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE67F1D4E
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = eirWork.dll
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








