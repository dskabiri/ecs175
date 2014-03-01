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

    void goToXY(double, double);
    void drawLine(double, double, double, double);
    void lineRel(double, double);
    void paint();
    void sierpA (int);
    void sierpB (int);
    void sierpC (int);
    void sierpD (int);
    
signals:
    
public slots:
    //void lineLoop();

   void setValue(int);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QGLShaderProgram* shader;
    GLuint vertex_array;
    GLuint vertex_buffer;
    void initializeShaders();
    void initializeBuffers();
    GLuint draw_mode;
    int valuez;
   double dist0;
   double dist;
   double x;
   double y;



};

#endif // GLWIDGET_H
