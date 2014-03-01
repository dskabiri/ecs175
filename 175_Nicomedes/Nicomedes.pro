#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T12:43:21
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = demo2
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    glwidget.cpp

HEADERS  += window.h \
    glwidget.h

OTHER_FILES += \
    basic.vert \
    basic.frag

RESOURCES += \
    shaders.qrc
