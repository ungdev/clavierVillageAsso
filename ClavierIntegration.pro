#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T16:35:51
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClavierIntegration
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    client.cpp \
    arduino.cpp

HEADERS  += mainwindow.h \
    server.h \
    client.h \
    arduino.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    res.qrc
