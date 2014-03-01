#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <vector>
#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>

using namespace std;

class GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    std::vector<vector<double> > find_cp(vector<vector<double> >);
    std::vector<vector<double> > gen_cp(int, vector<vector<double> >);
    void plotCartoon();
    
signals:
    
public slots:


   void setValue(int);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QGLShaderProgram* shader;
    GLuint vertex_array;
    GLuint vertex_buffer;

    GLuint draw_mode;
    int valuez;
    vector<vector<double> > cp_initial;
    vector<vector<double> > cp_gen;




};

#endif // GLWIDGET_H
