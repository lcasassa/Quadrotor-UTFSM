#-------------------------------------------------
#
# Project created by QtCreator 2011-04-26T23:10:10
#
#-------------------------------------------------

QT       += core gui

TARGET = iarc
TEMPLATE = app

CONFIG += QWT

SOURCES += main.cpp\
        mainwindow.cpp \
        plot.cpp \
        kalman.cpp \
    kalman2.cpp \
    serial.cpp \
    quadrotor.cpp


HEADERS  += mainwindow.h \
        plot.h \
        kalman.h \
    kalman2.h \
    serial.h \
    quadrotor.h

FORMS    += mainwindow.ui


# QWT
CONFIG(debug, debug|release) {
 win32: LIBS +=  -LC:/Qt/Qwt-6.0.0/lib/ -lqwtd
} else {
 win32: LIBS +=  -LC:/Qt/Qwt-6.0.0/lib/ -lqwt
}
 unix:LIBS += -lqwt -L/usr/local/qwt-6.0.1/lib/

 unix:INCLUDEPATH += /usr/local/qwt-6.0.1/include
 win32:INCLUDEPATH += C:/Qt/Qwt-6.0.0/include


#qextserialport
CONFIG(debug, debug|release) {
 win32: LIBS +=  -LC:/Qt/qextserialport-build-desktop/build/ -lqextserialportd
} else {
 win32: LIBS +=  -LC:/Qt/qextserialport-build-desktop/build/ -lqextserialport
}
 unix:LIBS += -lqextserialport

 unix:INCLUDEPATH += /usr/include
 win32:INCLUDEPATH += C:/Qt/qextserialport-1.2win-alpha
