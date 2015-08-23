#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T16:35:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClavierIntegration
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    server.h \
    client.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x
