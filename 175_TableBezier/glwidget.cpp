#include "glwidget.h"

#include <GL/glext.h>
#if defined(__linux)
#include <GL/glx.h>
#endif
#include <QtGui>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shapes.h"
//#include <glm/gtx/rotate_vector.hpp>
using namespace std;


PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(parent), draw_mode(GL_TRIANGLES), angle_x(0.f), angle_y(0.f), ctrl_pt0(glm::vec3(0.2, -1.3, 0.0)), ctrl_pt1(glm::vec3(.2, -0.5, 0.0)), ctrl_pt2(glm::vec3(.2, -0.5, 0.0)),ctrl_pt3(glm::vec3(1, -0.5, 0.0)), ctrl_pt4(glm::vec3(1, -0.5, 0.0)), ctrl_pt5(glm::vec3(1, -0.4, 0.0)), ctrl_pt6(glm::vec3(1, -0.4, 0.0)), ctrl_pt7(glm::vec3(0, -0.4, 0.0)),  selected_point(0)
{
  setFocusPolicy(Qt::StrongFocus);
  setFocus();

  cam_z = 3.0f;
}

GLWidget::~GLWidget()
{
  //delete the vertex array and vertex buffer
  GLuint loc = glGetAttribLocation(shader_lighting->programId(), "position");
  glDisableVertexAttribArray(loc);

  loc = glGetAttribLocation(shader_basic->programId(), "position");
  glDisableVertexAttribArray(loc);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &position_buffer_curve);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vertex_array_curve);

  //delete the shaders
  shader_lighting->release();
  shader_lighting->removeAllShaders();
  delete shader_lighting;

  shader_basic->release();
  shader_basic->removeAllShaders();
  delete shader_basic;
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

  //set point size
  glPointSize(4.0);

  //load and initialize shaders
  initializeShaders();

  //setup vertex and color data and send to GPU
  initializeBuffers();

  view_mat = glm::lookAt(glm::vec3(0,3,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_mat));


  //set the ambient lighting value
  GLfloat ambient_light = 0.f;
  loc = glGetUniformLocation(shader_lighting->programId(), "ambient");
  glUniform1f(loc, ambient_light);

  //set the specular coefficient
  GLfloat specular_coefficient = 5.f;
  loc = glGetUniformLocation(shader_lighting->programId(), "specular_coefficient");
  glUniform1f(loc, specular_coefficient);

  //set the light color
  loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);

  //light is not a point light
  loc = glGetUniformLocation(shader_lighting->programId(), "point_light");
  glUniform1i(loc, 0);

  //specular highlights off
  loc = glGetUniformLocation(shader_lighting->programId(), "specular_on");
  glUniform1i(loc, 0);
//  std::cout << (char*)glGetString(GL_VERSION) << std::endl;
}

void GLWidget::paintGL()
{
  //clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  proj_mat = glm::perspective(glm::pi<float>() * go_in, aspect_ratio, short_view, long_view);

  shader_lighting->bind();
  glBindVertexArray(vertex_array_surface);

  model_mat = glm::rotate(glm::mat4(1), glm::pi<float>() * angle_y, glm::vec3(0,1,0));
  view_mat = glm::lookAt(glm::vec3(cam_x+angle_y,cam_y+angle_x,cam_z), glm::vec3(cam_x,cam_y,1), glm::vec3(0,1,1));
  view_mat = glm::rotate(view_mat, glm::pi<float>() * angle_y, glm::vec3(1,0,0));
  view_mat = glm::rotate(view_mat, glm::pi<float>() * angle_x, glm::vec3(0,1,0));
   GLuint loccam = glGetUniformLocation(shader_lighting->programId(), "view_mat");
  glUniformMatrix4fv(loccam, 1, GL_FALSE, glm::value_ptr(view_mat));
  GLuint loc_model = glGetUniformLocation(shader_lighting->programId(), "model_mat");
  glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  normal_mat = glm::mat3(view_mat * model_mat);
  GLuint loc_norm = glGetUniformLocation(shader_lighting->programId(), "normal_mat");
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));
  GLuint loc_color = glGetUniformLocation(shader_lighting->programId(), "material_color");
  glUniform4f(loc_color, 0.8, 0.8, 0.8, 1.0);
  glDrawElements(GL_TRIANGLES, num_indices_surface, GL_UNSIGNED_BYTE, (GLvoid*)0);


//=========================
  glBindVertexArray(vertex_array_surface2);
  loc_model = glGetUniformLocation(shader_lighting->programId(), "model_mat");
  glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model_mat));

  loc_norm = glGetUniformLocation(shader_lighting->programId(), "normal_mat");
  glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));
 loc_color = glGetUniformLocation(shader_lighting->programId(), "material_color");
  glUniform4f(loc_color, 0.8, 0.8, 0.8, 1.0);
  glDrawElements(GL_TRIANGLES, num_indices_surface2, GL_UNSIGNED_BYTE, (GLvoid*)0);

//================================

   glBindVertexArray(vertex_array_surface3);
  model_mat = glm::translate(glm::mat4(1), glm::vec3(0.2,-0.66,0));

 loc_model = glGetUniformLocation(shader_lighting->programId(), "model_mat");
 glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model_mat));
 normal_mat = glm::mat3(view_mat * model_mat);
 loc_norm = glGetUniformLocation(shader_lighting->programId(), "normal_mat");
 glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));
 loc_color = glGetUniformLocation(shader_lighting->programId(), "material_color");
 glUniform4f(loc_color, 0.8, 0.0, 0.0, 1.0);
 glDrawElements(GL_TRIANGLES, num_indices_surface3, GL_UNSIGNED_BYTE, (GLvoid*)0);

//=============


 glBindVertexArray(vertex_array_surface4);
model_mat = glm::translate(glm::mat4(1), glm::vec3(0.6,-0.58,0));

loc_model = glGetUniformLocation(shader_lighting->programId(), "model_mat");
glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model_mat));
normal_mat = glm::mat3(view_mat * model_mat);
loc_norm = glGetUniformLocation(shader_lighting->programId(), "normal_mat");
glUniformMatrix3fv(loc_norm, 1, GL_FALSE, glm::value_ptr(normal_mat));
loc_color = glGetUniformLocation(shader_lighting->programId(), "material_color");
glUniform4f(loc_color, 0.25, 0.018, 0.0, 1.0);
glDrawElements(GL_TRIANGLES, num_indices_surface4, GL_UNSIGNED_BYTE, (GLvoid*)0);




  glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
  //set the viewing rectangle to be the entire window
  glViewport(0,0,w,h);
   aspect_ratio = w/(float)h;
  proj_mat = glm::perspective(glm::pi<float>() * 0.25f, aspect_ratio, 0.1f, 100.f);
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "proj_mat");
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
  shader_lighting = new QGLShaderProgram();
  shader_lighting->addShaderFromSourceFile(QGLShader::Vertex, ":/lighting.vert");
  shader_lighting->addShaderFromSourceFile(QGLShader::Fragment, ":/lighting.frag");
  shader_lighting->link();
  shader_lighting->bind();
  shader_basic = new QGLShaderProgram();
  shader_basic->addShaderFromSourceFile(QGLShader::Vertex, ":/basic.vert");
  shader_basic->addShaderFromSourceFile(QGLShader::Fragment, ":/basic.frag");
  shader_basic->link();
  shader_basic->bind();
}

void GLWidget::initializeBuffers()
{
  //---------------------------------------------------------------------------
  //curve
  //---------------------------------------------------------------------------
  std::vector<GLfloat> vertices;

  shader_basic->bind();
  //generate vertices, normals, and indices for the curve
 makeCubicBezierCurve(&vertices, point(ctrl_pt0.x, ctrl_pt0.y, ctrl_pt0.z), point(ctrl_pt1.x, ctrl_pt1.y, ctrl_pt1.z), point(ctrl_pt2.x, ctrl_pt2.y, ctrl_pt2.z),point(ctrl_pt3.x, ctrl_pt3.y, ctrl_pt3.z), 10);
  num_vertices_curve = vertices.size()/3;

  //add control points to end of curve
  vertices.push_back(ctrl_pt0.x);
  vertices.push_back(ctrl_pt0.y);
  vertices.push_back(ctrl_pt0.z);
  vertices.push_back(ctrl_pt1.x);
  vertices.push_back(ctrl_pt1.y);
  vertices.push_back(ctrl_pt1.z);
  vertices.push_back(ctrl_pt2.x);
  vertices.push_back(ctrl_pt2.y);
  vertices.push_back(ctrl_pt2.z);
  vertices.push_back(ctrl_pt3.x);
  vertices.push_back(ctrl_pt3.y);
  vertices.push_back(ctrl_pt3.z);

    //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_curve);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_curve);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_buffer_curve);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_curve);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  GLuint loc = glGetAttribLocation(shader_basic->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //---------------------------------------------------------------------------
  //plane  greeen plane
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //surface
  //---------------------------------------------------------------------------
  shader_lighting->bind();
  std::vector<GLfloat> vertices_normals;
  std::vector<GLubyte> indices;

  //generate vertices, normals, and indices for the cube


  makeCubicBezierSurface(&vertices_normals, &indices, point(ctrl_pt0.x, ctrl_pt0.y, ctrl_pt0.z), point(ctrl_pt1.x, ctrl_pt1.y, ctrl_pt1.z), point(ctrl_pt2.x, ctrl_pt2.y, ctrl_pt2.z),point(ctrl_pt3.x, ctrl_pt3.y, ctrl_pt3.z), 10);

      num_indices_surface = indices.size();

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_surface);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_surface);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_and_normal_buffer_surface);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_normals.size(), &vertices_normals[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3));


  glGenBuffers(1, &index_buffer_surface);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_surface);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices.size(), &indices[0], GL_STATIC_DRAW);



  //=----========
  std::vector<GLfloat> vertices_normals2;
  std::vector<GLubyte> indices2;
  makeCubicBezierSurface(&vertices_normals2, &indices2, point(ctrl_pt4.x, ctrl_pt4.y, ctrl_pt4.z), point(ctrl_pt5.x ,ctrl_pt5.y, ctrl_pt5.z), point(ctrl_pt6.x, ctrl_pt6.y, ctrl_pt6.z),point(ctrl_pt7.x, ctrl_pt7.y, ctrl_pt7.z), 10);

      num_indices_surface2 = indices2.size();

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_surface2);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_surface2);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_and_normal_buffer_surface2);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface2);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_normals2.size(), &vertices_normals2[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3));


  glGenBuffers(1, &index_buffer_surface2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_surface2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices2.size(), &indices2[0], GL_STATIC_DRAW);



  //=----========


  //=----========
  std::vector<GLfloat> vertices_normals3;
  std::vector<GLubyte> indices3;
  makeCubicBezierSurface(&vertices_normals3, &indices3, point(0.0, 0.25, 0.0), point(0.2 ,0.26,0.0), point(0.23, 0.5, 0.0),point(0.0, 0.42, 0.0), 10);

      num_indices_surface3 = indices3.size();

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_surface3);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_surface3);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_and_normal_buffer_surface3);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface3);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_normals3.size(), &vertices_normals3[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3));


  glGenBuffers(1, &index_buffer_surface3);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_surface3);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices3.size(), &indices3[0], GL_STATIC_DRAW);



  //=----========


  std::vector<GLfloat> vertices_normals4;
  std::vector<GLubyte> indices4;
  makeCubicBezierSurface(&vertices_normals4, &indices4, point(0.0, 0.17, 0.0), point(0.2 ,0.14,0.0), point(0, 0.3, 0.0),point(1.49e-09, 0.35, 0.0), 10);

      num_indices_surface4 = indices4.size();

  //get a unique id for the vertex array
  glGenVertexArrays(1, &vertex_array_surface4);
  //set the vertex array to be the active one
  glBindVertexArray(vertex_array_surface4);

  //get a unique id for the position vertex buffer
  glGenBuffers(1, &position_and_normal_buffer_surface4);
  //set the position vertex buffer to be active
  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface4);
  //specify the size and type of the position vertex buffer and load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_normals4.size(), &vertices_normals4[0], GL_STATIC_DRAW);

  //set the size of the position vector (3 component) and connect it to the "position" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "position");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

  //set the size of the normal vector (3 component) and connect it to the "normal" shader variable
  loc = glGetAttribLocation(shader_lighting->programId(), "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3));


  glGenBuffers(1, &index_buffer_surface4);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_surface4);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices4.size(), &indices4[0], GL_STATIC_DRAW);



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
    short_view -= 0.02f;

    break;
  }
  case Qt::Key_P:
  {
    short_view += 0.02f;

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

void GLWidget::selectP0()
{
    selected_point = 0;
}

void GLWidget::selectP1()
{
    selected_point = 1;
}

void GLWidget::selectP2()
{
    selected_point = 2;
}

void GLWidget::selectP3()
{
    selected_point = 3;
}

void GLWidget::selectP4()
{
    selected_point = 4;
}

void GLWidget::selectP5()
{
    selected_point = 5;
}

void GLWidget::selectP6()
{
    selected_point = 6;
}

void GLWidget::selectP7()
{
    selected_point = 7;
}

void GLWidget::increaseX()
{
   switch(selected_point)
   {
   case 0:
     ctrl_pt0.x += 0.1;
     break;
   case 1:
     ctrl_pt1.x += 0.1;
     break;
   case 2:
     ctrl_pt2.x += 0.1;
     break;
   case 3:
     ctrl_pt3.x += 0.1;
          ctrl_pt4.x += 0.1;
     break;
   case 4:
       ctrl_pt3.x += 0.1;
            ctrl_pt4.x += 0.1;
     break;
   case 5:
     ctrl_pt5.x += 0.1;
     break;
   case 6:
     ctrl_pt6.x += 0.1;
     break;
   case 7:
     ctrl_pt7.x += 0.1;
     break;


   }
   updateBezierCurve();
   updateGL();
}

void GLWidget::decreaseX()
{
  switch(selected_point)
   {
   case 0:
     ctrl_pt0.x -= 0.1;
     break;
   case 1:
     ctrl_pt1.x -= 0.1;
     break;
   case 2:
     ctrl_pt2.x -= 0.1;
     break;
  case 3:
      ctrl_pt3.x -= 0.1;
  ctrl_pt4.x -= 0.1;
    break;
  case 4:
        ctrl_pt3.x -= 0.1;
    ctrl_pt4.x -= 0.1;
    break;
  case 5:
    ctrl_pt5.x -= 0.1;
    break;
  case 6:
    ctrl_pt6.x -= 0.1;
    break;
  case 7:
    ctrl_pt7.x -= 0.1;
    break;
   }
   updateBezierCurve();
   updateGL();
}

void GLWidget::increaseY()
{
  switch(selected_point)
   {
   case 0:
     ctrl_pt0.y += 0.1;
     break;
   case 1:
     ctrl_pt1.y += 0.1;
     break;
   case 2:
     ctrl_pt2.y += 0.1;
     break;
  case 3:
    ctrl_pt3.y += 0.1;
    ctrl_pt4.y += 0.1;
    break;
  case 4:
      ctrl_pt3.y += 0.1;
      ctrl_pt4.y += 0.1;
    break;
  case 5:
    ctrl_pt5.y += 0.1;
    break;
  case 6:
    ctrl_pt6.y += 0.1;
    break;

  case 7:
    ctrl_pt7.y += 0.1;
    break;
   }
   updateBezierCurve();
   updateGL();
}

void GLWidget::decreaseY()
{
  switch(selected_point)
   {
   case 0:
     ctrl_pt0.y -= 0.1;
     break;
   case 1:
     ctrl_pt1.y -= 0.1;
     break;
   case 2:
     ctrl_pt2.y -= 0.1;
     break;
  case 3:
    ctrl_pt3.y -= 0.1;
    ctrl_pt4.y -= 0.1;
    break;
  case 4:
      ctrl_pt3.y -= 0.1;
    ctrl_pt4.y -= 0.1;
    break;
  case 5:
    ctrl_pt5.y -= 0.1;
    break;
  case 6:
    ctrl_pt6.y -= 0.1;
    break;
  case 7:
    ctrl_pt7.y -= 0.1;
    break;
   }
   updateBezierCurve();
   updateGL();
}

void GLWidget::updateBezierCurve()
{
  std::vector<GLfloat> vertices;
  makeCubicBezierCurve(&vertices, point(ctrl_pt0.x, ctrl_pt0.y, ctrl_pt0.z), point(ctrl_pt1.x, ctrl_pt1.y, ctrl_pt1.z), point(ctrl_pt2.x, ctrl_pt2.y, ctrl_pt2.z),point(ctrl_pt3.x, ctrl_pt3.y, ctrl_pt3.z), 10);
  num_vertices_curve = vertices.size()/3;



  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_curve);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertices.size(), &vertices[0]);

  vertices.clear();
  std::vector<GLfloat> normals;
  std::vector<GLubyte> indices;
  makeCubicBezierSurface(&vertices, &indices, point(ctrl_pt0.x, ctrl_pt0.y, ctrl_pt0.z), point(ctrl_pt1.x, ctrl_pt1.y, ctrl_pt1.z), point(ctrl_pt2.x, ctrl_pt2.y, ctrl_pt2.z),point(ctrl_pt3.x, ctrl_pt3.y, ctrl_pt3.z), 10);
  num_indices_surface = indices.size();

  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertices.size(), &vertices[0]);





//================2=============================



  std::vector<GLfloat> vertices2;

  std::vector<GLfloat> normals2;
  std::vector<GLubyte> indices2;
  makeCubicBezierSurface(&vertices2, &indices2, point(ctrl_pt4.x, ctrl_pt4.y, ctrl_pt4.z), point(ctrl_pt5.x, ctrl_pt5.y, ctrl_pt5.z), point(ctrl_pt6.x, ctrl_pt6.y, ctrl_pt6.z),point(ctrl_pt7.x, ctrl_pt7.y, ctrl_pt7.z), 10);
  num_indices_surface2 = indices2.size();

  glBindBuffer(GL_ARRAY_BUFFER, position_and_normal_buffer_surface2);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertices2.size(), &vertices2[0]);
}




void GLWidget::ambient(int value)
{
  GLfloat ambient_light = value/5.f;
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "ambient");
  glUniform1f(loc, ambient_light);
  updateGL();
}

void GLWidget::specular(int value)
{
  GLfloat specular_coefficient = 5*value + 5;
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "specular_coefficient");
  glUniform1f(loc, specular_coefficient);
  updateGL();
}

void GLWidget::lightIntensity(int value)
{
  light_intensity = value/5.f;
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::redLight()
{
  light_color = glm::vec3(1,0.2,0.2);
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::greenLight()
{
  light_color = glm::vec3(0.2,1,0.2);
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::blueLight()
{
  light_color = glm::vec3(0.2,0.2,1);
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::whiteLight()
{
  light_color = glm::vec3(1,1,1);
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "light_color");
  glUniform4f(loc, light_color.r*light_intensity, light_color.g*light_intensity, light_color.b*light_intensity, 1.f);
  updateGL();
}

void GLWidget::pointLight()
{
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "point_light");
  glUniform1i(loc, 1);
  updateGL();
}

void GLWidget::directionalLight()
{
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "point_light");
  glUniform1i(loc, 0);
  updateGL();
}

void GLWidget::specularOn(bool checked)
{
  GLuint loc = glGetUniformLocation(shader_lighting->programId(), "specular_on");
  glUniform1i(loc, checked);
  updateGL();
}


