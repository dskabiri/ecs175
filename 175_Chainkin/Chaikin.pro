#-------------------------------------------------
#
# Project created by QtCreator 2014-01-08T17:40:08
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    glwidget.cpp

HEADERS  += window.h \
    glwidget.h

RESOURCES += \
    shaders.qrc

OTHER_FILES += \
    basic.frag \
    basic.vert
