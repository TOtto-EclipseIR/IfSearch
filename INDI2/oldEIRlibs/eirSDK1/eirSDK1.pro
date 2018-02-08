#-------------------------------------------------
#
# Project created by QtCreator 2011-11-26T12:21:09
#
#-------------------------------------------------

QT       += network sql xml

TARGET = eirSDK1
TEMPLATE = lib

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe -leirFile -leirGUI

DEFINES += EIRSDK1_LIBRARY

SOURCES += eirSDK1.cpp \
    SdkMachine.cpp \
    SdkCommand.cpp \
    SdkClient-States.cpp \
    SdkClientParameters.cpp \
    SdkClient-Initialize.cpp \
    SdkClient-Command.cpp \
    SdkClient-Actions.cpp \
    SdkClient.cpp \
    ImageDirSink.cpp \
    PersonReportRow.cpp \
    SearchCommand.cpp \
    RetrieveCommand.cpp \
    RetrieveAllCommand.cpp

HEADERS += eirSDK1.h\
        eirSDK1_global.h \
    SdkMachine.h \
    SdkCommand.h \
    SdkClientParameters.h \
    SdkClient.h \
    ImageDirSink.h \
    PersonReportRow.h \
    SearchCommand.h \
    RetrieveCommand.h \
    RetrieveAllCommand.h

OTHER_FILES += \
    SdkMachineOverview.graphml \
    SdkMachine.state.violet \
    Command-Results.graphml \
    Command.state.violet \
    buildMe.pri








