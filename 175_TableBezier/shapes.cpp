#include "shapes.h"
#include <stdlib.h>
#include <iostream>
using namespace std;







void makeCubicBezierCurve(std::vector<GLfloat>* vertices, point p0, point p1, point p2, point p3, int num_segments)
{
    for(int i = 0; i <= num_segments; ++i)
    {
      float t = i/(float)num_segments;
      float one_minus_t = 1 - t;

      point p_on_curve = (p0 * one_minus_t * one_minus_t * one_minus_t) + (p1*(3 * one_minus_t * one_minus_t * t)) + (p2*(3 * one_minus_t * t * t)) + (p3*(t*t*t));
      vertices->push_back(p_on_curve.x);
      vertices->push_back(p_on_curve.y);
      vertices->push_back(p_on_curve.z);
    }
}

void makeCubicBezierCurveNormals(std::vector<GLfloat>* normals, point p0, point p1, point p2, point p3, int num_segments)
{
  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
    point derivative = (p1 - p0)*(3*one_minus_t*one_minus_t) + (p2-p1)*(6*one_minus_t*t) + (p3-p2)*(3*t*t);

    point n;
    n = point(derivative.y, -derivative.x, 0);
    n = n.normalize();

    normals->push_back(n.x);
    normals->push_back(n.y);
    normals->push_back(n.z);
  }
}

void makeCubicBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, point p3,  int num_segments)
{
    std::vector<GLfloat> temp_vertices, temp_normals;
    double theta = 0;
    double divisions = 10;
    double piDiv = 2*M_PI/divisions;
    makeCubicBezierCurveNormals(&temp_normals, p0, p1, p2, p3, num_segments);
    makeCubicBezierCurve(&temp_vertices, p0, p1, p2, p3, num_segments);


for(double k = 0; k <= divisions; k++){

    theta = k*piDiv;

    for(unsigned int j = 0; j < temp_vertices.size(); j += 3)
    {
        vertices_and_normals->push_back(temp_vertices[j]*cos(theta));
        vertices_and_normals->push_back(temp_vertices[j+1]);
        vertices_and_normals->push_back(-temp_vertices[j]*sin(theta));
        vertices_and_normals->push_back(temp_normals[j] * cos(theta));
        vertices_and_normals->push_back(temp_normals[j+1]);
        vertices_and_normals->push_back(-temp_normals[j] * sin(theta));
    }

    for(int i = 0; i < num_segments; ++i)
    {
      for(int j = 0; j < num_segments; ++j)
      {
        //triangle 1
        indices->push_back(i*(num_segments+1)+j);
        indices->push_back((i+1)*(num_segments+1)+j);
        indices->push_back(i*(num_segments+1)+j+1);
        //triangle 2
        indices->push_back((i+1)*(num_segments+1)+j);
        indices->push_back((i+1)*(num_segments+1)+j+1);
        indices->push_back(i*(num_segments+1)+j+1);
      }
    }

    }
}
