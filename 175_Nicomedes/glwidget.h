#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>

class GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void drawIt();
    void startIteration(GLfloat, GLfloat);
    void drawPoint(GLfloat, GLfloat, GLfloat);


signals:
    
public slots:


     void setValue(int);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QGLShaderProgram* shader;
    GLuint vertex_array1;
    GLuint vertex_buffer1;

    GLuint draw_mode;
    double drawPoints;
    unsigned int num_vertices1;
    unsigned int num_vertices2;

    
};

#endif
