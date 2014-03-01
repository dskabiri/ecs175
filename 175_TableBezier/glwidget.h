#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>
#include <glm/glm.hpp>
#include <vector>
//#include <glm/gtx/rotate_vector.hpp>

class GLWidget : public QGLWidget, protected QGLFunctions
{
  Q_OBJECT
public:
  explicit GLWidget(QWidget *parent = 0);
  ~GLWidget();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

signals:

public slots:
  void selectP0();
  void selectP1();
  void selectP2();
  void selectP3();
    void selectP4();
      void selectP5();
      void selectP6();
        void selectP7();
  void increaseX();
  void decreaseX();
  void increaseY();
  void decreaseY();
  void ambient(int value);
  void specular(int value);
  void lightIntensity(int value);
  void redLight();
  void greenLight();
  void blueLight();
  void whiteLight();
  void pointLight();
  void directionalLight();
  void specularOn(bool checked);



protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);
  void keyPressEvent(QKeyEvent *e);

private:
  void initializeShaders();
  void initializeBuffers();
  void updateBezierCurve();

  QGLShaderProgram* shader_lighting;
  QGLShaderProgram* shader_basic;
  GLuint vertex_array_curve;
  GLuint position_buffer_curve;
  GLuint vertex_array_surface;
  GLuint vertex_array_surface2;
   GLuint vertex_array_surface3;
      GLuint vertex_array_surface4;
  GLuint position_and_normal_buffer_surface;
  GLuint position_and_normal_buffer_surface2;
    GLuint position_and_normal_buffer_surface3;
      GLuint position_and_normal_buffer_surface4;

  GLuint index_buffer_surface;
  GLuint index_buffer_surface2;
    GLuint index_buffer_surface3;
 GLuint index_buffer_surface4;
  GLuint vertex_array_plane;
  GLuint position_buffer_plane;
  GLuint draw_mode;
  glm::mat4 proj_mat;
  glm::mat4 view_mat;
  glm::mat4 model_mat;
  //glm::vec3 rotate_mat;
  glm::mat3 normal_mat;
  float angle_x;
  float angle_y;
  float angle_z;
  float cam_x;
  float cam_y;
  float cam_z;
  float go_in;
  float go_out;
  float aspect_ratio;
  float short_view;
  float long_view;
  int num_vertices_curve;
  int num_indices_surface;
  int num_vertices_curve2;
  int num_indices_surface2;
  int num_vertices_curve3;
    int num_indices_surface3;
    int num_vertices_curve4;
      int num_indices_surface4;
  glm::vec3 ctrl_pt0;
  glm::vec3 ctrl_pt1;
  glm::vec3 ctrl_pt2;
  glm::vec3 ctrl_pt3;
   glm::vec3 ctrl_pt4;
    glm::vec3 ctrl_pt5;
   glm::vec3 ctrl_pt6;
   glm::vec3 ctrl_pt7;
  int selected_point;

  glm::vec3 light_color;
  GLfloat light_intensity;
};

#endif // GLWIDGET_H
