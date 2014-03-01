#include "window.h"

#include <QtGui>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    glwidget = new GLWidget();
    QSlider* s_button = new QSlider(Qt::Horizontal);
       s_button -> setRange(0,200);
       s_button -> setSingleStep(1);
       connect(s_button, SIGNAL(valueChanged(int)), glwidget, SLOT(setValue(int)));



    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(glwidget);
    vbox->addWidget(s_button);


    setLayout(vbox);
}
