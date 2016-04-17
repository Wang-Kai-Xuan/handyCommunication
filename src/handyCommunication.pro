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
    about.cpp \
    menu.cpp \
    broadcast.cpp \
    user_tree.cpp \
    udp.cpp

HEADERS  += \
    start.h \
    ilabel.h \
    about.h \
    menu.h \
    broadcast.h \
    user_tree.h \
    header.h \
    udp.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resource.qrc

RC_FILE = app.rc

DISTFILES += \
    readme.txt \
    question.txt \
    notes.txt
