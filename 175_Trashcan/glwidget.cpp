#include "glwidget.h"

#include <GL/glext.h>
#include <GL/glx.h>

#include <QtGui>
#include <iostream>
//using namespace std;
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shapes.h"


using namespace std;

PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent), draw_mode(GL_TRIANGLES), angle_x(0.f), angle_y(0.f), light_color(glm::vec3(1,1,1)), light_intensity(1),
{
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  go_in = 0.25f;
  cam_x = 0.25f;
  cam_y = 0.25f;
  cam_z = 10.f;
  angle_x =  0.0f;
  angle_y = 0.0f;
  short_view = 0.1f;
  long_view = 100.f;
}

GLWidget::~GLWidget()
{
  //delete the vertex array and vertex buffer
  GLuint loc = glGetAttribLocation(shader->programId(), "position");
  glDisableVertexAttribArray(loc);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &position_buffer_cube);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &index_buffer_cube);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vertex_array_cube);

  //delete the shaders
  shader->release();
  shader->removeAllShaders();
  delete shader;
}

void GLWidget::initializeGL()
{
  //load opengl functions that are not in QGLFunctions
#if defined(__linux)
  glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glDeleteVertexArrays");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glGenVertexArrays");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((GLubyte*)"glBindVertexArray");
#elif defined(WIN32)
  glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
#endif

  //load opengl functions
  initializeGLFunctions();

  //set the clear color to black
  glClearColor(0,0,0,1);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);

  //enable back face culling
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //load and initialize shaders
  initializeShaders();

  //setup vertex and color data and send to GPU
  initializeBuffers();

  view_mat = glm::lookAt(glm::vec3(0,3,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
  GLuint loc = glGetUniformLocation(shader->programId(), "view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_mat));

  //set the ambient lighting value
  GLfloat ambient_light = 0.f;
  loc = glGetUniformLocation(shader->programId(), "ambient");
  glUniform1f(loc, ambient_light);

  //set the specular coefficient
  GLfloat specular_coefficient = 5.f;
  loc = glGetUniformLocation(shader->programId(), "specular_coefficient");
  glUniform1f(loc, specular_coefficient);

  //set the light color
  loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);

  //light is not a point light
  loc = glGetUniformLocation(shader->programId(), "point_light");
  glUniform1i(loc, 0);

  //specular highlights off
  loc = glGetUniformLocation(shader->programId(), "specular_on");
  glUniform1i(loc, 0);
}

void GLWidget::paintGL()
{

  //clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  proj_mat = glm::perspective(glm::pi<float>() * go_in, aspect_ratio, short_view, long_view);
  GLuint locz = glGetUniformLocation(shader->programId(), "proj_mat");
  glUniformMatrix4fv(locz, 1, GL_FALSE, glm::value_ptr(proj_mat));

  view_mat = glm::lookAt(glm::vec3(cam_x+angle_y,cam_y+angle_x,cam_z), glm::vec3(cam_x,cam_y,1), glm::vec3(0,1,0));
  view_mat = glm::rotate(view_mat, glm::pi<float>() * angle_y, glm::vec3(1,0,0));
  view_mat = glm::rotate(view_mat, glm::pi<float>() * angle_x, glm::vec3(0,1,0));
  GLuint loccam = glGetUniformLocation(shader->programId(), "view_mat");
  glUniformMatrix4fv(loccam, 1, GL_FALSE, glm::value_ptr(view_mat));

  //cylinder
  glBindVertexArray(vertex_array_holy_cylinder);
  //model_mat = glm::translate(glm::mat4(1), glm::vec3(-5,0,0));
  model_mat = glm::rotate(glm::mat4(1), glm::pi<float>() * 0.5f, glm::vec3(1,0,0));
// model_mat = glm::scale(model_mat, glm::vec3(1,0.1,2));
  GLuint loc = glGetUniformLocation(shader->programId(), "model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  normal_mat = glm::mat3(view_mat*model_mat);
  GLuint loc_norm = glGetUniformLocation(shader->programId(), "normal_mat");
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

  //set a new color red
  GLuint loc_color = glGetUniformLocation(shader->programId(), "material_color");
  glUniform4f(loc_color, 0.0f, 0.8f, 0.8f, 1.f);

  GLuint loc_spec_color = glGetUniformLocation(shader->programId(), "material_specular_color");
  glUniform4f(loc_spec_color, 1.f, 1.f, 1.f, 1.f);


  glDrawArrays(draw_mode, 0, num_vertices_holy_cylinder);


    //----------------------------------------------------------------------------------
  //base

  glBindVertexArray(vertex_array_cylinder);

  model_mat = glm::translate(glm::mat4(1), glm::vec3(0,-1,0));
  model_mat = glm::rotate(model_mat, glm::pi<float>() * 0.5f, glm::vec3(1,0,0));
  model_mat = glm::scale(model_mat, glm::vec3(.85,.85,.85));
  loc = glGetUniformLocation(shader->programId(), "model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  normal_mat = glm::mat3(view_mat*model_mat);
  loc_norm = glGetUniformLocation(shader->programId(), "normal_mat");
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

  //set a new color red
  loc_color = glGetUniformLocation(shader->programId(), "material_color");
  glUniform4f(loc_color, 0.0f, 0.8f, 0.8f, 1.f);

  loc_spec_color = glGetUniformLocation(shader->programId(), "material_specular_color");
  glUniform4f(loc_spec_color, 1.f, 1.f, 1.f, 1.f);


  glDrawArrays(draw_mode, 0, num_vertices_cylinder);
  //----------------------------------------------------------------------------------

  //top

glBindVertexArray(vertex_array_cylinder);

model_mat = glm::translate(glm::mat4(1), glm::vec3(0,1,0));
model_mat = glm::rotate(model_mat, glm::pi<float>() * 0.5f, glm::vec3(1,0,0));
model_mat = glm::scale(model_mat, glm::vec3(.9,.9,0.9));

loc = glGetUniformLocation(shader->programId(), "model_mat");
glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

normal_mat = glm::mat3(view_mat*model_mat);
loc_norm = glGetUniformLocation(shader->programId(), "normal_mat");
glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

//set a new color red
loc_color = glGetUniformLocation(shader->programId(), "material_color");
glUniform4f(loc_color, 0.0f, 0.8f, 0.8f, 1.f);

loc_spec_color = glGetUniformLocation(shader->programId(), "material_specular_color");
glUniform4f(loc_spec_color, 1.f, 1.f, 1.f, 1.f);


glDrawArrays(draw_mode, 0, num_vertices_cylinder);

    //--------inside cylinder----------------------------------------------------------------

  glBindVertexArray(vertex_array_cylinder);

  model_mat = glm::rotate(glm::mat4(1), glm::pi<float>() * 0.5f, glm::vec3(1,0,0));
  model_mat = glm::scale(model_mat, glm::vec3(0.70,0.65,10));

  loc = glGetUniformLocation(shader->programId(), "model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  normal_mat = glm::mat3(view_mat*model_mat);
  loc_norm = glGetUniformLocation(shader->programId(), "normal_mat");
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

  //set a new color red
  loc_color = glGetUniformLocation(shader->programId(), "material_color");
  glUniform4f(loc_color, 0.0f, 0.4f, 0.5f, 1.f);

  loc_spec_color = glGetUniformLocation(shader->programId(), "material_specular_color");
  glUniform4f(loc_spec_color, 1.f, 1.f, 1.f, 1.f);


  glDrawArrays(draw_mode, 0, num_vertices_cylinder);

  //----------------------------------------------------------------------------------

  //Draw Sphere
  //-----------------------------------------------------------------------------------

  //switch to the sphere
  glBindVertexArray(vertex_array_sphere);

  model_mat = glm::translate(glm::mat4(1), glm::vec3(0,1.075,0));
 // model_mat = glm::rotate(model_mat, glm::pi<float>() * 0.8f, glm::vec3(1,0,0));

  model_mat = glm::scale(model_mat, glm::vec3(0.75,0.75,0.75));
  model_mat = glm::rotate(model_mat, glm::pi<float>() * 0.5f, glm::vec3(0,-0.3,0));
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  //non-uniform scaling in the model matrix -- need to use inverse transpose of the modelview matrix
  normal_mat = glm::transpose(glm::inverse(glm::mat3(view_mat*model_mat)));


  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

  //set a new color
  glUniform4f(loc_color, 0.f, 0.8f, 0.8f, 1.f);

  //draw the sphere
  glDrawArrays(draw_mode, 0, num_vertices_sphere);



  //switch to the plane
  glBindVertexArray(vertex_array_plane);

  model_mat = glm::mat4(1);
  model_mat = glm::translate(glm::mat4(1), glm::vec3(0,-0.25,0));
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  normal_mat = glm::mat3(view_mat*model_mat);
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));

  //set a new color
  glUniform4f(loc_color, 0.5f, 0.5f, 0.5f, 1.f);

  //draw the plane
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);




    //-------------------------------------------------------------------------------------------------


}

void GLWidget::resizeGL(int w, int h)
{

  //set the viewing rectangle to be the entire window
  glViewport(0,0,w,h);
  aspect_ratio = w/(float)h;
  proj_mat = glm::perspective(glm::pi<float>() * 0.25f, aspect_ratio, 0.1f, 100.f);
  GLuint loc = glGetUniformLocation(shader->programId(), "proj_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj_mat));
}

QSize GLWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
  return QSize(500, 500);
}

void GLWidget::initializeShaders()
{
  //load the shaders, link the shader, and set the shader to be active
  shader = new QGLShaderProgram();
  shader->addShaderFromSourceFile(QGLShader::Vertex, ":/lighting.vert");
  shader->addShaderFromSourceFile(QGLShader::Fragment, ":/lighting.frag");
  shader->link();
  shader->bind();
}

void GLWidget::initializeBuffers()
{
  //---------------------------------------------------------------------------
  //cube
  //---------------------------------------------------------------------------
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> normals;
  std::vector<GLubyte> indices;



  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  GLuint loc = glGetAttribLocation(shader->programId(), "position");



 //===============================================================================================

  vertices.clear();
  normals.clear();

  MakeCylinder(30, 0.12, 1.21,&vertices, &normals);
  num_vertices_cylinder = vertices.size()/3;

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_cylinder);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_cylinder);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_buffer_cylinder);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_cylinder);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //get a unique id for the normal vertex buffer
  glGenBuffers(1, &normal_buffer_cylinder);
  //set the normal vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_cylinder);
  //specify the size and type of the normal vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), &normals[0], GL_STATIC_DRAW);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------

  vertices.clear();
  normals.clear();

  MakecutCylinder(60, 1, 1, 30, &vertices, &normals);
  num_vertices_holy_cylinder = vertices.size()/3;

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_holy_cylinder);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_holy_cylinder);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_buffer_holy_cylinder);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_holy_cylinder);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //get a unique id for the normal vertex buffer
  glGenBuffers(1, &normal_buffer_holy_cylinder);
  //set the normal vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_holy_cylinder);
  //specify the size and type of the normal vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), &normals[0], GL_STATIC_DRAW);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //---------------------------------------------------------------------------
  //sphere
  //---------------------------------------------------------------------------
  vertices.clear();
  normals.clear();

  makeSphere(&vertices, &normals, 5);
  num_vertices_sphere = vertices.size()/3;

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_sphere);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_sphere);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_buffer_sphere);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_sphere);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //get a unique id for the normal vertex buffer
  glGenBuffers(1, &normal_buffer_sphere);
  //set the normal vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_sphere);
  //specify the size and type of the normal vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), &normals[0], GL_STATIC_DRAW);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);




  //---------------------------------------------------------------------------
  //plane
  //---------------------------------------------------------------------------
  vertices.clear();
  normals.clear();

  vertices.push_back(-2); vertices.push_back(-1); vertices.push_back(2);
  vertices.push_back(2); vertices.push_back(-1); vertices.push_back(2);
  vertices.push_back(-2); vertices.push_back(-1); vertices.push_back(-2);
  vertices.push_back(2); vertices.push_back(-1); vertices.push_back(-2);

  normals.push_back(0); normals.push_back(1); normals.push_back(0);
  normals.push_back(0); normals.push_back(1); normals.push_back(0);
  normals.push_back(0); normals.push_back(1); normals.push_back(0);
  normals.push_back(0); normals.push_back(1); normals.push_back(0);

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_plane);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_plane);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_buffer_plane);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_plane);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //get a unique id for the normal vertex buffer
  glGenBuffers(1, &normal_buffer_plane);
  //set the normal vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_plane);
  //specify the size and type of the normal vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), &normals[0], GL_STATIC_DRAW);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Right:
      {
        cam_x += 0.1f;

        break;
      }

    case Qt::Key_Left:
      {
        cam_x -= 0.1f;

        break;
      }

    case Qt::Key_Up:
      {
        cam_y += 0.1f;

        break;
      }

    case Qt::Key_Down:
      {
        cam_y -= 0.1f;

        break;
      }


    case Qt::Key_C:
      {
        cam_z -= 0.1f;

        break;
      }

    case Qt::Key_V:
      {
        cam_z += 0.1f;

        break;
      }

    case Qt::Key_W:
      {
        angle_y += 0.1f;

        break;
      }

    case Qt::Key_S:
      {
        angle_y -= 0.1f;

        break;
      }

    case Qt::Key_A:
      {
        angle_x -= 0.1f;

        break;
      }

    case Qt::Key_D:
      {
        angle_x += 0.1f;

        break;
      }


    case Qt::Key_L:
      {
        glPolygonMode(GL_FRONT, GL_LINE);
        break;
      }

    case Qt::Key_F:
      {
        glPolygonMode(GL_FRONT, GL_FILL);
        break;
      }
    case Qt::Key_Z:
    {
      go_in -= 0.02f;

      break;
    }
    case Qt::Key_X:
    {
      go_in += 0.02f;

      break;
    }
    case Qt::Key_O:
    {
      if(short_view > 0.05f)
      short_view -= 0.2f;

      break;
    }
    case Qt::Key_P:
    {
      short_view += 0.2f;

      break;
    }
    case Qt::Key_J:
    {
      if(long_view > 0.05f && long_view > short_view + 6)
      long_view -= 2.f;

      break;
    }
    case Qt::Key_K:
    {
      if(long_view < 100.f)
      long_view += 2.f;

      break;
    }




  default:
    QWidget::keyPressEvent(e);
    break;
  }

  updateGL();
}
void GLWidget::ambient(int value)
{
  GLfloat ambient_light = value/5.f;
  GLuint loc = glGetUniformLocation(shader->programId(), "ambient");
  glUniform1f(loc, ambient_light);
  updateGL();
}

void GLWidget::specular(int value)
{
  GLfloat specular_coefficient = 5*value + 5;
  GLuint loc = glGetUniformLocation(shader->programId(), "specular_coefficient");
  glUniform1f(loc, specular_coefficient);
  updateGL();
}

void GLWidget::lightIntensity(int value)
{
  light_intensity = value/5.f;
  GLuint loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::redLight()
{
  light_color = glm::vec3(1,0.2,0.2);
  GLuint loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::greenLight()
{
  light_color = glm::vec3(0.2,1,0.2);
  GLuint loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::blueLight()
{
  light_color = glm::vec3(0.2,0.2,1);
  GLuint loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::whiteLight()
{
  light_color = glm::vec3(1,1,1);
  GLuint loc = glGetUniformLocation(shader->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::pointLight()
{
  GLuint loc = glGetUniformLocation(shader->programId(), "point_light");
  glUniform1i(loc, 1);
  updateGL();
}

void GLWidget::directionalLight()
{
  GLuint loc = glGetUniformLocation(shader->programId(), "point_light");
  glUniform1i(loc, 0);
  updateGL();
}

void GLWidget::specularOn(bool checked)
{
  GLuint loc = glGetUniformLocation(shader->programId(), "specular_on");
  glUniform1i(loc, checked);
  updateGL();
}

