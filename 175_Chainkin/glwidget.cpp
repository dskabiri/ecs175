#include "glwidget.h"
#include <cmath>
#include <iostream>
#include <vector>

#include <QtGui>

PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

using namespace std;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)

{
        valuez = 0;


      plotCartoon();

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
         glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
       glEnable(GL_DEPTH_TEST);
       glDepthFunc(GL_LEQUAL);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();



    glLineWidth(3);
    glColor3f(1,0.0,0.7);
    glBegin(GL_LINE_LOOP);

    for (int i=0; i < cp_gen.size(); i++)
    {
        double x = cp_gen[i][0];
        double y = cp_gen[i][1];
        glVertex2f((GLfloat)x, (GLfloat)y);

    }
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

     cp_gen = gen_cp(value, cp_initial);

     updateGL();

}

vector<vector<double> > GLWidget::gen_cp(int num_iters, vector<vector<double> > points)
{

    for (int i = 0; i<num_iters; i++) {
        points = find_cp(points);

    }

    return points;
}



//I found this code online on pastebin.com it was very helpful.
    vector<vector<double> > GLWidget::find_cp(vector<vector<double> > initial_points)
{
    vector<vector<double> > result_points;
    for (int i = 0; i < initial_points.size(); i++){// we iterate over all of the sequential pairs of points

        //initialize the pair of points
        vector<double> start_p = initial_points[i];
        vector<double> end_p = initial_points[((i + 1) % initial_points.size())];

        //find 1/4 point and 3/4 point of the segment made by these points
        vector<double> begin_mark (2);
        vector<double> end_mark (2);

        begin_mark[0] = start_p[0] + (end_p[0] - start_p[0])/4;
        begin_mark[1] = start_p[1] + (end_p[1] - start_p[1])/4;

        end_mark[0] = start_p[0] + 3*(end_p[0] - start_p[0])/4;
        end_mark[1] = start_p[1] + 3*(end_p[1] - start_p[1])/4;

        //add these 1/4 and 3/4 points to our results vector
        result_points.push_back(begin_mark);
        result_points.push_back(end_mark);
    }
    return result_points;
}

void  GLWidget::plotCartoon(){

        //initializing the points  I worked with a team to figure out how we would plot the points
        vector<double> A (2);
        A[0] = 0;
        A[1] = .94;
        vector<double> B (2);
        B[0] = -0.14;
        B[1] = 0.63;
        vector<double> C (2);
        C[0] = -0.35;
        C[1] = 0.1;
        vector<double> D (2);
        D[0] = -0.35;
        D[1] = 0.1;
        vector<double> E (2);
        E[0] = -0.75;
        E[1] = 0.43;
        vector<double> F (2);
        F[0] = -0.45;
        F[1] = -0.08;
        vector<double> G (2);
        G[0] = -0.45;
        G[1] = -0.08;
        vector<double> H (2);
        H[0] = -0.6;
        H[1] = -0.4;
        vector<double> I (2);
        I[0] = -0.4;
        I[1] = -0.7;
        vector<double> J (2);
        J[0] = -0.4;
        J[1] = -0.7;
        vector<double> K (2);
        K[0] = -0.3;
        K[1] = -1.1;
        vector<double> L (2);
        L[0] = -0.12;
        L[1] = -0.7;
        vector<double> M (2);
        M[0] = -0.12;
        M[1] = -0.7;
        vector<double> N (2);
        N[0] = -0.0;
        N[1] = -0.7;
        vector<double> O (2);
        O[0] = -0.0;
        O[1] = -0.7;
        vector<double> P (2);
        P[0] = 0.42;
        P[1] = -1.1;
        vector<double> Q (2);
        Q[0] = 0.32;
        Q[1] = -0.63;
        vector<double> R (2);
        R[0] = 0.32;
        R[1] = -0.63;
        vector<double> S (2);
        S[0] = 0.42;
        S[1] = -0.33;
        vector<double> T (2);
        T[0] = 0.32;
        T[1] = -0.05;
        vector<double> U (2);
        U[0] = 0.32;
        U[1] = -0.05;
        vector<double> V (2);
        V[0] = 0.7;
        V[1] = 0.22;
        vector<double> W (2);
        W[0] = 0.2;
        W[1] = 0.12;
        vector<double> X (2);
        X[0] = 0.2;
        X[1] = 0.12;
        cp_initial.push_back(A);
        cp_initial.push_back(B);
        cp_initial.push_back(C);
        cp_initial.push_back(D);
        cp_initial.push_back(E);
        cp_initial.push_back(F);
        cp_initial.push_back(G);
        cp_initial.push_back(H);
        cp_initial.push_back(I);
        cp_initial.push_back(J);
        cp_initial.push_back(K);
        cp_initial.push_back(L);
        cp_initial.push_back(M);
        cp_initial.push_back(N);
        cp_initial.push_back(O);
        cp_initial.push_back(P);
        cp_initial.push_back(Q);
        cp_initial.push_back(R);
        cp_initial.push_back(S);
        cp_initial.push_back(T);
        cp_initial.push_back(U);
        cp_initial.push_back(V);
        cp_initial.push_back(W);cp_initial.push_back(X);
        cp_gen = gen_cp(valuez, cp_initial);
    }

