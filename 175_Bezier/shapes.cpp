#include "shapes.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

void makeQuadraticBezierCurve(std::vector<GLfloat>* vertices, point p0, point p1, point p2, int num_segments)
{
  // number of evaluated points is number of segments + 1
  // quadratic bezier formula = ((1-t)^2)p0 + (2(1-t)t)p1 + (t^2)p2
  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
    point p_on_curve = (p0 * one_minus_t * one_minus_t) + (p1 * (2*one_minus_t*t)) + (p2 * t * t);
    vertices->push_back(p_on_curve.x);
    vertices->push_back(p_on_curve.y);
    vertices->push_back(p_on_curve.z);
  }
}

void makeCubicBezierCurve(std::vector<GLfloat>* vertices, point p0, point p1, point p2, point p3, int num_segments){
    //cubic equation = ((1-t)^3)p0 + 3((1-t)^2)p1 + 3((1-t)^2)p2 + (t^3)p3

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

void makeQuadraticBezierCurveNormals(std::vector<GLfloat>* normals, point p0, point p1, point p2, int num_segments)
{
  // number of evaluated points is number of segments + 1
  // quadratic bezier formula derivative = (2*(1-t))(p1 - p0) + 2t(p2-p1)
  // formula for normal is (-y'(t), x(t)
  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
    point derivative = (p1 - p0)*(2*one_minus_t) + (p2-p1)*(2*t);
    point n(-derivative.y, derivative.x, 0);
    n = n.normalize();
    normals->push_back(n.x);
    normals->push_back(n.y);
    normals->push_back(n.z);
  }
}

void makeCubicBezierCurveNormals(std::vector<GLfloat>* normals, point p0, point p1, point p2, point p3, int num_segments)
{
  // number of evaluated points is number of segments + 1
  // cubic bezier formula derivative = 3(1-t)^2(p1 - p0) + 6(1-t)t(p2-p1) +3t^2(p3-p2)
  // formula for normal is (-y'(t), x(t)
  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
    point derivative = (p1 - p0)*(3*one_minus_t*one_minus_t) + (p2-p1)*(6*one_minus_t*t) + (p3-p2)*(3*t*t);
    point n(derivative.x, 0, derivative.z);
    n = n.normalize();
    normals->push_back(n.x);
    normals->push_back(n.y);
    normals->push_back(n.z);
  }
}

void makeQuadraticBezierDisplayNormals(std::vector<GLfloat>* vertices, point p0, point p1, point p2, int num_segments)
{
  // number of evaluated points is number of segments + 1
  // quadratic bezier formula = ((1-t)^2)p0 + (2(1-t)t)p1 + (t^2)p2

  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
    point p_on_curve = (p0 * one_minus_t * one_minus_t) + (p1 * (2*one_minus_t*t)) + (p2 * t * t);
    point derivative = (p1 - p0)*(2*one_minus_t) + (p2-p1)*(2*t);
    point n(-derivative.y, derivative.x, 0);
    n = n.normalize();
    vertices->push_back(p_on_curve.x);
    vertices->push_back(p_on_curve.y);
    vertices->push_back(p_on_curve.z);
    vertices->push_back(p_on_curve.x + n.x);
    vertices->push_back(p_on_curve.y + n.y);
    vertices->push_back(p_on_curve.z + n.z);
  }
}


void makeCubicBezierDisplayNormals(std::vector<GLfloat>* vertices, point p0, point p1, point p2, point p3, int num_segments)

{
  for(int i = 0; i <= num_segments; ++i)
  {
    float t = i/(float)num_segments;
    float one_minus_t = 1 - t;
     point p_on_curve = (p0 * one_minus_t * one_minus_t * one_minus_t) + (p1*(3 * one_minus_t * one_minus_t * t)) + (p2*(3 * one_minus_t * t * t)) + (p3*(t*t*t));
    point derivative = (p1 - p0)*(3*one_minus_t*one_minus_t) + (p2-p1)*(6*one_minus_t*t) + (p3-p2)*(3*t*t);
    point n(derivative.x, 0, derivative.z);
    n = n.normalize();
    vertices->push_back(p_on_curve.x);
    vertices->push_back(p_on_curve.y);
    vertices->push_back(p_on_curve.z);
    vertices->push_back(p_on_curve.x + n.x);
    vertices->push_back(p_on_curve.y + n.y);
    vertices->push_back(p_on_curve.z + n.z);
  }
}

void makeBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, int num_segments)
{
  std::vector<GLfloat> curve_vertices;
  std::vector<GLfloat> curve_normals;
  makeQuadraticBezierCurve(&curve_vertices, p0, p1, p2, num_segments);
  makeQuadraticBezierCurveNormals(&curve_normals, p0, p1, p2, num_segments);
  //extrude in the z direction
  for(int i = 0; i <= num_segments; ++i)
  {
    //loop over curve vertices
    for(unsigned int j = 0; j < curve_vertices.size(); j += 3)
    {
      float offset = 2.0*i/(float)num_segments;
      vertices_and_normals->push_back(curve_vertices[j]);
      vertices_and_normals->push_back(curve_vertices[j+1]);
      vertices_and_normals->push_back(curve_vertices[j+2] + offset);
      vertices_and_normals->push_back(curve_normals[j]);
      vertices_and_normals->push_back(curve_normals[j+1]);
      vertices_and_normals->push_back(curve_normals[j+2]);
    }
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

/*

void makeCubicBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, point p3,  int num_segments)
{
  std::vector<GLfloat> curve_vertices;
  std::vector<GLfloat> curve_normals;
  makeCubicBezierCurve(&curve_vertices, p0, p1, p2, p3, num_segments);
  makeCubicBezierCurveNormals(&curve_normals, p0, p1, p2, p3, num_segments);
  //extrude in the z direction
  for(int i = 0; i <= num_segments; ++i)
  {
    //loop over curve vertices
    for(unsigned int j = 0; j < curve_vertices.size(); j += 3)
    {
      cout << curve_vertices[j] << endl;
      float offset = 1.0*i/(float)num_segments;


      vertices_and_normals->push_back(curve_vertices[j]);
      vertices_and_normals->push_back(curve_vertices[j+1]);
      vertices_and_normals->push_back(curve_vertices[j+2]+offset); //offest is what extrudes the plane.
      vertices_and_normals->push_back(curve_normals[j]);
      vertices_and_normals->push_back(curve_normals[j+1]);
      vertices_and_normals->push_back(curve_normals[j+2]);
    }
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
*/

void makeCubicBezierSurface(std::vector<GLfloat>* vertices_and_normals, std::vector<GLubyte>* indices, point p0, point p1, point p2, point p3,  int num_segments)
{

    std::vector<GLfloat> curve_vertices, curve_normals;


    double theta = 0;
    double divisions = 10;
    double piDiv = 2*M_PI/divisions;
    double temp0[3] = {0,0,0};
    double temp1[3] = {0,0,0};
    double temp2[3] = {0,0,0};
    double temp3[3] = {0,0,0};
    double temp20[3] = {0,0,0};
    double temp21[3] = {0,0,0};
    double temp22[3] = {0,0,0};
    double temp23[3] = {0,0,0};


for(double k = 0; k <= divisions; k++){
    std::vector<GLfloat> temp_vertices, temp2_vertices, temp_normals, temp2_normals;
    theta = k*piDiv;
    double transition[3][3] = {{cos(theta), 0, sin(theta)}, {0, 1, 0}, {-sin(theta), 0, cos(theta)}};

    for (int i=0;i<3;i++){
             temp0[i]=0;
             temp1[i]=0;
             temp2[i]=0;
             temp3[i]=0;
             temp20[i]=0;
             temp21[i]=0;
             temp22[i]=0;
             temp23[i]=0;
    }

    for (int i=0;i<3;i++){
          temp0[i]+=( p0.x * transition[i][0]);
          temp1[i]+=( p1.x * transition[i][0]);
          temp2[i]+=( p2.x * transition[i][0]);
          temp3[i]+=( p3.x * transition[i][0]);
          temp0[i]+=( p0.y * transition[i][1]);
          temp1[i]+=( p1.y * transition[i][1]);
          temp2[i]+=( p2.y * transition[i][1]);
          temp3[i]+=( p3.y * transition[i][1]);
          temp0[i]+=( p0.z * transition[i][2]);
          temp1[i]+=( p1.z * transition[i][2]);
          temp2[i]+=( p2.z * transition[i][2]);
          temp3[i]+=( p3.z * transition[i][2]);
     }
     for (int i=0;i<2;i++){
          temp20[i]+=( p0.x * transition[i+1][0]);
          temp21[i]+=( p1.x * transition[i+1][0]);
          temp22[i]+=( p2.x * transition[i+1][0]);
          temp23[i]+=( p3.x * transition[i+1][0]);
          temp20[i]+=( p0.y * transition[i+1][1]);
          temp21[i]+=( p1.y * transition[i+1][1]);
          temp22[i]+=( p2.y * transition[i+1][1]);
          temp23[i]+=( p3.y * transition[i+1][1]);
          temp20[i]+=( p0.z * transition[i+1][2]);
          temp21[i]+=( p1.z * transition[i+1][2]);
          temp22[i]+=( p2.z * transition[i+1][2]);
          temp23[i]+=( p3.z * transition[i+1][2]);
    }

     temp20[2]+=( p0.x * transition[0][0]);
     temp21[2]+=( p1.x * transition[0][0]);
     temp22[2]+=( p2.x * transition[0][0]);
     temp23[2]+=( p3.x * transition[0][0]);
     temp20[2]+=( p0.y * transition[0][1]);
     temp21[2]+=( p1.y * transition[0][1]);
     temp22[2]+=( p2.y * transition[0][1]);
     temp23[2]+=( p3.y * transition[0][1]);
     temp20[2]+=( p0.z * transition[0][2]);
     temp21[2]+=( p1.z * transition[0][2]);
     temp22[2]+=( p2.z * transition[0][2]);
     temp23[2]+=( p3.z * transition[0][2]);


     makeCubicBezierCurve(&temp_vertices, point(temp0[0], temp0[1], temp0[2]), point(temp1[0], temp1[1], temp1[2]), point(temp2[0], temp2[1], temp2[2]),point(temp3[0], temp3[1], temp3[2]), num_segments);
     makeCubicBezierCurveNormals(&temp_normals, point(temp0[0], temp0[1], temp0[2]), point(temp1[0], temp1[1], temp1[2]), point(temp2[0], temp2[1], temp2[2]),point(temp3[0], temp3[1], temp3[2]), num_segments);

     makeCubicBezierCurve(&temp2_vertices, point(temp20[0], temp20[1], temp20[2]), point(temp21[0], temp21[1], temp21[2]), point(temp22[0], temp22[1], temp22[2]),point(temp23[0], temp23[1], temp23[2]), num_segments);
     makeCubicBezierCurveNormals(&temp2_normals, point(temp20[0], temp20[1], temp20[2]), point(temp21[0], temp21[1], temp21[2]), point(temp22[0], temp22[1], temp22[2]),point(temp23[0], temp23[1], temp23[2]), num_segments);





    for(unsigned int j = 0; j < temp_vertices.size(); j += 3)
    {
        vertices_and_normals->push_back(temp_vertices[j]);
        vertices_and_normals->push_back(temp2_vertices[j]);
        vertices_and_normals->push_back(temp2_vertices[j+1]);
        vertices_and_normals->push_back(temp_normals[j]);
        vertices_and_normals->push_back(temp2_normals[j]);
        vertices_and_normals->push_back(temp2_normals[j+1]);
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


    curve_vertices.insert(curve_vertices.end(), temp_vertices.begin(), temp_vertices.end());
    curve_normals.insert(curve_normals.end(), temp_normals.begin(), temp_normals.end());
    }
}
