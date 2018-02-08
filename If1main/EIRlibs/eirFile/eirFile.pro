#-------------------------------------------------
#
# Project created by QtCreator 2011-12-04T12:10:17
#
#-------------------------------------------------

QT       += network xml

TARGET = eirFile
TEMPLATE = lib

DEFINES += EIRFILE_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes -leirCore -leirExe

SOURCES += eirFile.cpp \
    QQDir.cpp \
    TemporaryDir.cpp \
    CsvWriter.cpp \
    CsvReaderMachine.cpp \
    CsvReader.cpp

HEADERS += eirFile.h\
        eirFile_global.h \
    QQDir.h \
    TemporaryDir.h \
    CsvWriter.h \
    CsvReaderMachine.h \
    CsvReader.h

OTHER_FILES += \
    useFile.pri \
    CsvReaderMachine.graphml \
    buildMe.pri







