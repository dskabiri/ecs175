#ifndef SHAPES_H_
#define SHAPES_H_

#include <vector>
#include <cmath>
#include <qgl.h>

void makeCube(std::vector<GLfloat>* vertices, std::vector<GLfloat>* normals, std::vector<GLubyte>* indices);
void makeSphere(std::vector<GLfloat>* vertices, std::vector<GLfloat>* normals, int num_subdivisions);
void MakeCylinder(unsigned int numSteps, int length, std::vector<GLfloat>* vertices,  std::vector<GLfloat>* normals);

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
};

struct tri
{
  point p1, p2, p3;
  tri(point a, point b, point c) { p1 = a; p2 = b; p3 = c; }
};


#endif // SHAPES_H_
