#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T20:55:04
#
#-------------------------------------------------

QT       += core gui
QT       += core network #модуль для работы с сетью


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server1
TEMPLATE = app


SOURCES += main.cpp\
    myserver.cpp \
    client.cpp

HEADERS  += \
    myserver.h \
    client.h

FORMS    += mainwindow.ui
