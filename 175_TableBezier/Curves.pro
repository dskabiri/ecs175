#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T14:31:03
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Bezier
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    glwidget.cpp \
    shapes.cpp

HEADERS  += window.h \
    glwidget.h \
    shapes.h

INCLUDEPATH +=  $$quote($(GLM_PATH))

RESOURCES += \
    shaders.qrc
