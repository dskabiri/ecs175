#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>
#include <glm/glm.hpp>
#include <vector>

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

  QGLShaderProgram* shader;
  GLuint vertex_array_cube;
  GLuint position_buffer_cube;
  GLuint normal_buffer_cube;
  GLuint index_buffer_cube;
  GLuint vertex_array_cylinder;
  GLuint normal_buffer_cylinder;
  GLuint position_buffer_cylinder;
  GLuint vertex_array_cones;
  GLuint normal_buffer_cones;
  GLuint position_buffer_cones;
  GLuint vertex_array_sphere;
  GLuint position_buffer_sphere;
  GLuint normal_buffer_sphere;
  GLuint vertex_array_plane;
  GLuint position_buffer_plane;
  GLuint normal_buffer_plane;
  GLuint draw_mode;
  glm::mat4 proj_mat;
  glm::mat4 view_mat;
  glm::mat4 model_mat;
  glm::mat3 normal_mat;
  float angle_x;
  float angle_y;
  float cam_x;
  float cam_y;
  float cam_z;
  float go_in;
  float go_out;
  float short_view;
  float long_view;
  float aspect_ratio;
  int num_vertices_sphere;
  int num_vertices_cylinder;
  int num_vertices_cones;
  glm::vec3 light_color;
  GLfloat light_intensity;
};

#endif // GLWIDGET_H
