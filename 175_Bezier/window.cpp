#include "window.h"

#include <QtGui>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    glwidget = new GLWidget();
    QGroupBox* point_group = new QGroupBox("current control point");
    QRadioButton* p0 = new QRadioButton("p0");
    QRadioButton* p1 = new QRadioButton("p1");
    QRadioButton* p2 = new QRadioButton("p2");
    QRadioButton* p3 = new QRadioButton("p3");
    QRadioButton* p4 = new QRadioButton("p4");
    QRadioButton* p5 = new QRadioButton("p5");
    QRadioButton* p6 = new QRadioButton("p6");
    QRadioButton* p7 = new QRadioButton("p7");
    p0->setChecked(true);
    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(p0);
    hbox->addWidget(p1);
    hbox->addWidget(p2);
    hbox->addWidget(p3);
    hbox->addWidget(p4);
    hbox->addWidget(p5);
    hbox->addWidget(p6);
    hbox->addWidget(p7);

    point_group->setLayout(hbox);

    QGroupBox* move_group = new QGroupBox("move points");
    QPushButton* increaseX = new QPushButton("increase x");
    QPushButton* decreaseX = new QPushButton("decrease x");
    QPushButton* increaseY = new QPushButton("increase y");
    QPushButton* decreaseY = new QPushButton("decrease y");
    QHBoxLayout* hbox2 = new QHBoxLayout();
    hbox2->addWidget(increaseX);
    hbox2->addWidget(decreaseX);
    hbox2->addWidget(increaseY);
    hbox2->addWidget(decreaseY);
    move_group->setLayout(hbox2);
    
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(glwidget);
    vbox->addWidget(point_group);
    vbox->addWidget(move_group);

    connect(p0, SIGNAL(clicked()), glwidget, SLOT(selectP0()));
    connect(p1, SIGNAL(clicked()), glwidget, SLOT(selectP1()));
    connect(p2, SIGNAL(clicked()), glwidget, SLOT(selectP2()));
    connect(p3, SIGNAL(clicked()), glwidget, SLOT(selectP3()));
    connect(p4, SIGNAL(clicked()), glwidget, SLOT(selectP4()));
    connect(p5, SIGNAL(clicked()), glwidget, SLOT(selectP5()));
    connect(p6, SIGNAL(clicked()), glwidget, SLOT(selectP6()));
    connect(p7, SIGNAL(clicked()), glwidget, SLOT(selectP7()));

    connect(increaseX, SIGNAL(clicked()), glwidget, SLOT(increaseX()));
    connect(decreaseX, SIGNAL(clicked()), glwidget, SLOT(decreaseX()));
    connect(increaseY, SIGNAL(clicked()), glwidget, SLOT(increaseY()));
    connect(decreaseY, SIGNAL(clicked()), glwidget, SLOT(decreaseY()));
    setLayout(vbox);
}

void Window::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWidget::keyPressEvent(e);
}
