#-------------------------------------------------
#
# Project created by QtCreator 2012-05-06T11:41:36
#
#-------------------------------------------------

QT       += core gui

TARGET = qDebug
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp \
    quadrotor.cpp \
    protocol.cpp \
    plot.cpp \
    configquadrotor.cpp

HEADERS  += mainwindow.h \
    serial.h \
    quadrotor.h \
    protocol.h \
    plot.h \
    configquadrotor.h

FORMS    += mainwindow.ui \
    configquadrotor.ui

#qextserialport
include(../../qextserialport-1.2beta1/src/qextserialport.pri)

#qwt
unix:!macx:!symbian: LIBS += -L/usr/local/qwt-6.0.2-svn/lib/ -lqwt

INCLUDEPATH += /usr/local/qwt-6.0.2-svn/include
DEPENDPATH += /usr/local/qwt-6.0.2-svn/include
