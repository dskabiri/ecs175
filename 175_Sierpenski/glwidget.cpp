#include "glwidget.h"


#include <QtGui>
#include <iostream>

PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

using namespace std;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)

{
    dist0 = 0.5;
       x = -1;
       y = -1;
     valuez = 1;

}

GLWidget::~GLWidget()
{
    //delete the vertex array and vertex buffer
    GLuint loc = glGetAttribLocation(shader->programId(), "position");
    glDisableVertexAttribArray(loc);
    loc = glGetAttribLocation(shader->programId(), "color");
    glDisableVertexAttribArray(loc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertex_buffer);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertex_array);

    //delete the shaders
    shader->release();
    shader->removeAllShaders();
    delete shader;
}

void GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL()
{
    //clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    //draw the 3 points as a triangle or line loop
    //glDrawArrays(draw_mode, 0, 3);
    paint();
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

valuez = value;
updateGL();

}
            // GRABBED THIS PART ONLINE IT WAS REALLY HELPFUL
void GLWidget::goToXY(double x_new, double y_new)
{
    glTranslated(x_new, y_new, 0.0);
}

void GLWidget::drawLine(double x1, double y1, double x2, double y2)
{

    glColor3f(1.0f,0.0f,0.7f);
    glBegin(GL_LINES);

    glVertex2f((GLfloat)x1, (GLfloat)y1); // origin of the line
   glVertex2f((GLfloat)x2, (GLfloat)y2); // ending point of the line

   glEnd();

}

void GLWidget::lineRel(double deltaX, double deltaY) {

    drawLine(x, y, x+deltaX, y+deltaY);
    x += deltaX;
    y += deltaY;
}

void GLWidget::paint() {

    dist = dist0;
    for (int i=valuez; i > 0; i--) dist /= 2;
    goToXY(2*dist, dist);
    sierpA(valuez);        // start recursion
    lineRel(+dist, +dist);
    sierpB(valuez);        // start recursion
    lineRel(-dist, +dist);
    sierpC(valuez);        // start recursion
    lineRel(-dist, -dist);
    sierpD(valuez);        // start recursion
    lineRel(+dist, -dist);
}

void GLWidget::sierpA (int level) {
    if (level > 0) {
        sierpA(level-1);

        lineRel(+dist, +dist);
        sierpB(level-1);
        lineRel(+2*dist, 0);
        sierpD(level-1);
        lineRel(+dist, -dist);
        sierpA(level-1);
    }
}

void GLWidget::sierpB (int level) {
    if (level > 0) {
        sierpB(level-1);
        lineRel(-dist, +dist);
        sierpC(level-1);
        lineRel(0, +2*dist);
        sierpA(level-1);
        lineRel(+dist, +dist);
        sierpB(level-1);
    }
}

void GLWidget::sierpC (int level) {
    if (level > 0) {
        sierpC(level-1);
        lineRel(-dist, -dist);
        sierpD(level-1);
        lineRel(-2*dist, 0);
        sierpB(level-1);
        lineRel(-dist, +dist);
        sierpC(level-1);
    }
}

void GLWidget::sierpD (int level) {
    if (level > 0) {
        sierpD(level-1);
        lineRel(+dist, -dist);
        sierpA(level-1);
        lineRel(0, -2*dist);
        sierpC(level-1);
        lineRel(-dist, -dist);
        sierpD(level-1);
    }
}


