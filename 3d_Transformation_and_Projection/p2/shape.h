
#ifndef POLY_H
#define POLY_H

#include <stdio.h>

typedef struct Point
{
	float x,y,z;
}Pt;

class Poly{
 public:
  Poly();
  void addvert(float x, float y, float z);
  float getMin();
  float getMax();
  int numV;
  void rotate(Pt p1, Pt p2, float theta);
  void translate(float x, float y, float z);
  void scale(float scal);
  void findvert(float *x, float *y, float *z, int n);
  void matrix4x4PreMultiply(float A[], float B[], float C[]);
  void matrix4x4SetIdentity(float A[], float B[], float C[]);

  float vert[5*10][3];
  void centroidX (float *x);
  void centroidY (float *y);
  void centroidZ (float *z);

};
#endif 