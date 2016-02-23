#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T14:16:29
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtChatServer_UDP
TEMPLATE = app


SOURCES += main.cpp \
    chat.cpp \
    start.cpp \
    ilabel.cpp \
    ioframe.cpp

HEADERS  += \
    chat.h \
    start.h \
    ilabel.h \
    ioframe.h \
    common.h

CONFIG += mobility
MOBILITY = 

