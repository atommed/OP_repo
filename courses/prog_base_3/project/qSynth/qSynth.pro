#-------------------------------------------------
#
# Project created by QtCreator 2016-06-04T23:02:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qSynth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pabackend.cpp \
    testcallback.cpp \
    spinlock.cpp

HEADERS  += mainwindow.h \
    pabackend.h \
    iaudiobackend.h \
    iaudiocallback.h \
    testcallback.h \
    constants.h \
    spinlock.h \
    iaudioeffect.h \
    igenericinput.h

FORMS    += mainwindow.ui

CONFIG += c++14
CONFIG += link_pkgconfig
PKGCONFIG += portaudio-2.0
