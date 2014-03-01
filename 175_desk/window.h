#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui/QWidget>
#include "glwidget.h"

class Window : public QWidget
{
  Q_OBJECT

public:
  Window(QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent *e);

private:
  GLWidget* glwidget;
};

#endif // WINDOW_H
