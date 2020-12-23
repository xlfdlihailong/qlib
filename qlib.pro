QT += core widgets
QT -= gui

CONFIG += c++11

TARGET = qlib
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    mainqlib.cpp \
    plib/tinyxmlparser.cpp \
    plib/tinyxmlerror.cpp \
    plib/tinyxml.cpp \
    plib/tinystr.cpp \
    plib/pxml.cpp \
    plib/pmysql.cpp \
    plib/plib.cpp \
    plib/clib.c \
    qlib.cpp

HEADERS += \
    plib/pmysql.h \
    plib/pxml.h \
    plib/tinystr.h \
    plib/tinyxml.h \
    plib/plib.h \
    plib/clib.h \
    qlib.h
LIBS+=-lpthread
LIBS+=-lmysqlclient
