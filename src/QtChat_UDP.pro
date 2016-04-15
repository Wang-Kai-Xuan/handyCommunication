#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T14:16:29
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SipleCommunucation
TEMPLATE = app


SOURCES += main.cpp \
    start.cpp \
    ilabel.cpp \
    ioframe.cpp \
    about.cpp \
    menu.cpp \
    secretletter.cpp \
    chatbase.cpp \
    broadcast.cpp \
    groupchat.cpp

HEADERS  += \
    start.h \
    ilabel.h \
    ioframe.h \
    common.h \
    about.h \
    menu.h \
    secretletter.h \
    chatbase.h \
    broadcast.h \
    groupchat.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resource.qrc

RC_FILE = app.rc

DISTFILES += \
    readme.txt \
    question.txt
