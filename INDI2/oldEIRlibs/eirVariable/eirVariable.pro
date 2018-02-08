#-------------------------------------------------
#
# Project created by QtCreator 2011-10-10T18:35:39
#
#-------------------------------------------------

QT       += sql xml

TARGET = eirVariable
TEMPLATE = lib

include(../libs.pri)
LIBS += -leirKeyId -leirTypes

DEFINES += EIRVARIABLE_LIBRARY

SOURCES += eirVariable.cpp \
    VariableId.cpp \
    VariableIdBehavior.cpp \
    Variable.cpp \
    VariableGroup.cpp \
    SettingsVariableGroup.cpp \
    ObjectVariableGroup.cpp \
    CsvReader.cpp \
    CsvReaderMachine.cpp \
    VariableTable.cpp \
    CsvWriter.cpp \
    VariableType.cpp \
    VariableHeader.cpp

HEADERS += eirVariable.h\
        eirVariable_global.h \
    VariableId.h \
    VariableIdBehavior.h \
    Variable.h \
    VariableGroup.h \
    SettingsVariableGroup.h \
    ObjectVariableGroup.h \
    CsvReader.h \
    CsvReaderMachine.h \
    VariableTable.h \
    CsvWriter.h \
    VariableType.h \
    VariableHeader.h










