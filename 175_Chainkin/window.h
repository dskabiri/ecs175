#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "glwidget.h"

class Window : public QWidget
{
    Q_OBJECT
    
public:
    Window(QWidget *parent = 0);
    GLWidget* glwidget;


};

#endif // WINDOW_H
