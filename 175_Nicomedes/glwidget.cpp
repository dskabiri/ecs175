#include "glwidget.h"

#include <GL/glext.h>
#include <GL/glx.h>
#include <QtGui>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

using namespace std;

PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent)
{
    drawPoints = -2;
     glColor3f(1,0.0,0.7);
}

GLWidget::~GLWidget()
{
  //delete the vertex array and vertex buffer
  GLuint loc = glGetAttribLocation(shader->programId(), "position");
  glDisableVertexAttribArray(loc);
  loc = glGetAttribLocation(shader->programId(), "color");
  glDisableVertexAttribArray(loc);

  glBindBuffer(GL_ARRAY_BUFFER, 0);



  shader->release();
  shader->removeAllShaders();
  delete shader;
}

void GLWidget::initializeGL()
{
  //load opengl functions that are not in QGLFunctions
  //on linux use glXGetProcAddress; on windows wglGetProcAddress
  glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glDeleteVertexArrays");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glGenVertexArrays");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((GLubyte*)"glBindVertexArray");

  //load opengl functions
  initializeGLFunctions();


  glClearColor(1,1,1,1);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);


}


void GLWidget::paintGL()
{
  //clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBegin(GL_POINTS);

    drawIt();
    glEnd();

}

void GLWidget::resizeGL(int w, int h)
{
  //set the viewing rectangle to be the entire window
  glViewport(0,0,w,h);
}

QSize GLWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
  return QSize(500, 500);
}


void GLWidget::setValue(int value){

     drawPoints = double(value)/50.000 -2.000;


    updateGL();

}



void GLWidget::drawIt() {
  GLfloat b = 0.5;


    startIteration(drawPoints, b);
}

void GLWidget::startIteration(GLfloat a, GLfloat b) {
  GLfloat theta;
  for (theta = 0; theta <= 2 * M_PI; theta += (M_PI / 8000)) {
    drawPoint(a, b, theta);

  }
     drawPoint(a, b, theta);
}

void GLWidget::drawPoint(GLfloat a, GLfloat b, GLfloat theta) {

  GLfloat r = b + a * (1.0 / cos(theta));
  GLfloat x = r * cos(theta);
  GLfloat y = r * sin(theta);
   glColor3f(1,0.0,0.7);
  glVertex2f(x, y);
}



