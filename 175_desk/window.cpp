#include "window.h"

#include <QtGui>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    glwidget = new GLWidget();
    QLabel* ambient_label = new QLabel("ambient");
    QSlider* ambient_slider = new QSlider();
    ambient_slider->setOrientation(Qt::Horizontal);
    ambient_slider->setRange(0, 5);
    QCheckBox* specular_on = new QCheckBox("specular");
    QSlider* specular_slider = new QSlider();
    specular_slider->setOrientation(Qt::Horizontal);
    specular_slider->setRange(0, 5);
    QLabel* light_label = new QLabel("light intensity");
    QSlider* light_slider = new QSlider();
    light_slider->setOrientation(Qt::Horizontal);
    light_slider->setRange(0, 5);
    light_slider->setSliderPosition(5);
    QGroupBox* light_group = new QGroupBox("light color");
    QRadioButton* red = new QRadioButton("red");
    QRadioButton* green = new QRadioButton("green");
    QRadioButton* blue = new QRadioButton("blue");
    QRadioButton* white = new QRadioButton("white");
    white->setChecked(true);
    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(red);
    hbox->addWidget(green);
    hbox->addWidget(blue);
    hbox->addWidget(white);
    light_group->setLayout(hbox);
    QGroupBox* light_type = new QGroupBox("light type");
    QRadioButton* point = new QRadioButton("point light");
    QRadioButton* direction = new QRadioButton("directional light");
    direction->setChecked(true);
    QHBoxLayout* hbox2 = new QHBoxLayout();
    hbox2->addWidget(point);
    hbox2->addWidget(direction);
    light_type->setLayout(hbox2);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(glwidget);
    vbox->addWidget(ambient_label);
    vbox->addWidget(ambient_slider);
    vbox->addWidget(specular_on);
    vbox->addWidget(specular_slider);
    vbox->addWidget(light_label);
    vbox->addWidget(light_slider);
    vbox->addWidget(light_group);
    vbox->addWidget(light_type);
    connect(ambient_slider, SIGNAL(valueChanged(int)), glwidget, SLOT(ambient(int)));
    connect(specular_slider, SIGNAL(valueChanged(int)), glwidget, SLOT(specular(int)));
    connect(light_slider, SIGNAL(valueChanged(int)), glwidget, SLOT(lightIntensity(int)));
    connect(red, SIGNAL(clicked()), glwidget, SLOT(redLight()));
    connect(green, SIGNAL(clicked()), glwidget, SLOT(greenLight()));
    connect(blue, SIGNAL(clicked()), glwidget, SLOT(blueLight()));
    connect(white, SIGNAL(clicked()), glwidget, SLOT(whiteLight()));
    connect(point, SIGNAL(clicked()), glwidget, SLOT(pointLight()));
    connect(direction, SIGNAL(clicked()), glwidget, SLOT(directionalLight()));
    connect(specular_on, SIGNAL(clicked(bool)), glwidget, SLOT(specularOn(bool)));
    setLayout(vbox);
}

void Window::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWidget::keyPressEvent(e);
}
