#ifndef SHAPES_H_
#define SHAPES_H_

#include <vector>
#include <cmath>
#include <qgl.h>
#include <glm/gtx/rotate_vector.hpp>

struct point
{
  float x, y, z;
  point() { x = 0; y = 0; z = 0; }
  point(float a, float b, float c) { x = a; y = b; z = c; }
  point normalize()
  {
    float length = sqrt(x*x + y*y + z*z);
    point norm = point(x/length, y/length, z/length);
    return norm;
  }
  point mid(point other)
  {
    float a = 0.5*(this->x + other.x);
    float b = 0.5*(this->y + other.y);
    float c = 0.5*(this->z + other.z);
    return point(a,b,c);
  }
  point operator = (point other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  point operator * (float scalar)
  {
    point pnew(scalar * x, scalar * y, scalar * z);
    return pnew;
  }
  point operator + (point other)
  {
    point pnew(x + other.x, y + other.y, z + other.z);
    return pnew;
  }
  point operator - (point other)
  {
    point pnew(x - other.x, y - other.y, z - other.z);
    return pnew;
  }
};

struct tri
{
  point p1, p2, p3;
  tri(point a, point b, point c) { p1 = a; p2 = b; p3 = c; }
};

void makeQuadraticBezierCurve(std::vector<GLfloat>* vertices, point p0, point p1, point p2, int num_segments);
void makeBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, int num_segments);
void makeQuadraticBezierDisplayNormals(std::vector<GLfloat>* vertices, point p0, point p1, point p2, int num_segments);

void makeCubicBezierCurve(std::vector<GLfloat>* vertices, point p0, point p1, point p2, point p3, int num_segments);
void makeCubicBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, point p3, int num_segments);
void makeCubicBezierDisplayNormals(std::vector<GLfloat>* vertices, point p0, point p1, point p2, point p3,  int num_segments);
void makeRotationBezier(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, point p3,  int num_segments);


#endif // SHAPES_H_
