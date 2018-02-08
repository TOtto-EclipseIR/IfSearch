#-------------------------------------------------
#
# Project created by QtCreator 2011-10-31T14:09:57
#
#-------------------------------------------------

#QT       -= gui

TARGET = eirCore
TEMPLATE = lib

DEFINES += EIRCORE_LIBRARY

include(../libs.pri)
LIBS *= -leirBase -leirTypes

SOURCES += eirCore.cpp \
    VariableType.cpp \
    VariableTable.cpp \
    VariableId.cpp \
    VariableHeader.cpp \
    VariableGroup.cpp \
    Variable.cpp \
    VariableIdList.cpp \
    VariableSet.cpp \
    BasicId.cpp \
    IdGenerator.cpp \
    VariableSettings.cpp \
    VariableSettingsList.cpp \
    Entity.cpp \
    IsTrigger.cpp

HEADERS += eirCore.h\
        eirCore_global.h \
    VariableType.h \
    VariableTable.h \
    VariableId.h \
    VariableHeader.h \
    VariableGroup.h \
    Variable.h \
    VariableIdList.h \
    VariableSet.h \
    BasicId.h \
    IdGenerator.h \
    VariableSettings.h \
    VariableSettingsList.h \
    Entity.h \
    IsTrigger.h

OTHER_FILES += \
    buildMe.pri



































