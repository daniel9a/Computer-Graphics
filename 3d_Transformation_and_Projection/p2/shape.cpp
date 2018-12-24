
#include <iostream>
#include "shape.h"
#include <cmath>
using namespace std;

Poly::Poly(){
  numV = 0; //Initialize Poly
}

void Poly::addvert(float x, float y, float z){
  vert[numV][0] = x;
  vert[numV][1] = y; //Add a new vertex
  vert[numV][2] = z;
  numV+=1;
}

float Poly::getMin(){ //Calculate the total minimum value
  float min=vert[0][0];
  int i=0;
  while(i < numV) {
    if (vert[i][0]<min) { min = vert[i][0]; }
    else if (vert[i][1]<min) { min = vert[i][1]; }
    else if (vert[i][2]<min) { min = vert[i][2]; }
    i++;
  }
  return min;
}

float Poly::getMax(){ //Calculate the total maximum value (for MasterMax)
  float max=vert[0][0];
  int i=0;
  while(i < numV) {
    if (vert[i][0]>max) { max = vert[i][0]; }
    else if (vert[i][1]>max) { max = vert[i][1]; }
    else if (vert[i][2]>max) { max = vert[i][2]; }
    i++;
  }
  return max;
}

void Poly::rotate(Pt p1, Pt p2, float theta){
  float dz, dy, dx;
  dz = p2.z-p1.z;
  dy = p2.y-p1.y;
  dx = p2.x-p1.x;
  float M[16];
  float l = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));
  float dl = l*l;
  float ddx = dx*dx;
  float in42=1/(dl+ddx);
  float in41=pow(dz,2)+pow(dy,2);
  float M1[] = {1,0,0,0,0,1,0,0,0,0,1,0,-p1.x,-p1.y,-p1.z,1};
  float Mxyz[] = {1,0,0,0,0,dz/l,dy/l,0,0,-dy/l,dz/l,0,0,0,0,1}; //Rotate over x-axis to YZ plane
  float Myz[] = {l,0,dx,0,0,1,0,0,-dx,0,l,0,0,0,0,1}; //Rotate over Y axis to Z axis
  float Mz[] = {float (cos(theta)),float (sin(theta)),0,0,float (-sin(theta)),float (cos(theta)),0,0,0,0,1,0,0,0,0,1}; //Rotate about Z
  float MzInv[]= {l*in42,0,-dx*in42,0,0,1,0,0,dx*in42,0,l*in42,0,0,0,0,1}; //Inverse of Mz
  float MxyzInv[]= {1,0,0,0,0,l*dz/in41,-l*dy/in41,0,0,l*dy/in41,l*dz/in41,0,0,0,0,1}; //Inverse of MXYZ
  float M5[] = {1,0,0,0,0,1,0,0,0,0,1,0,p1.x,p1.y,p1.z,1};
  matrix4x4PreMultiply(M5, MxyzInv,M);
  matrix4x4PreMultiply(M, MzInv,M5);
  matrix4x4PreMultiply(M5,Mz,M);
  matrix4x4PreMultiply(M,Myz,M5);
  matrix4x4PreMultiply(M5,Mxyz,M); //Create one single matrix
  matrix4x4PreMultiply(M,M1,M5);
  int i = 0;
  while(i < numV){
    float in_vert[] = {vert[i][0],vert[i][1],vert[i][2],1};
    float out_vert[4];
    matrix4x4SetIdentity(M5, in_vert, out_vert); //Apply rotation matrix to vertex
    vert[i][2]=out_vert[2];
    vert[i][1]=out_vert[1];
    vert[i][0]=out_vert[0];
    i++;
  }

}

void Poly::translate(float x, float y, float z){
  int i = 0;
  while(i < numV){
    vert[i][0]+=x; //Translation function
    vert[i][1]+=y;
    vert[i][2]+=z;
    i++;
  }
}

void Poly::scale(float s){
  float x,y,z;
  centroidX(&x);
  centroidY(&y);
  centroidZ(&z);
  translate(-x,-y,-z);
  int i = 0;
  while(i < numV){
    vert[i][0]*=s;
    vert[i][1]*=s;
    vert[i][2]*=s;
    i++;
  }
  translate(x,y,z);
}

void Poly::findvert(float *x, float *y, float *z, int n){
  *x=vert[n][0];
  *y=vert[n][1]; //Returns the nth vertex
  *z=vert[n][2];
}

void Poly::centroidX(float *x) {
  *x = 0;
  int i =0;
  while(i<numV){
    *x+=vert[i][0];
    i++;
  }
  *x = *x/numV;
}

void Poly::centroidY(float *y) {
  *y = 0;
  int i =0;
  while(i<numV){
    *y+=vert[i][0];
    i++;
  }
  *y = *y/numV;
}

void Poly::centroidZ(float *z) {
  *z = 0;
  int i =0;
  while(i<numV){
    *z+=vert[i][0];
    i++;
  }
  *z = *z/numV;
}

void Poly::matrix4x4PreMultiply(float A[], float B[],float C[]){ //4x4 x 4x4 matrix multiplier

  int i=0;
  while(i<4){
    int j=0;
    while (j<4){
      C[i*4+j]=0;
      int k=0;
      while(k<4){
        C[i*4+j]+=A[j+k*4]*B[i*4+k];
        k++;
      } j++;
    } i++;
  }

}

void Poly::matrix4x4SetIdentity(float A[], float B[], float C[]){ //4x4 x 4x1 matrix multiplier

  int i=0;
  while(i<4){
    C[i]=0;
    int j=0;
    while(j<4){
      C[i]+=A[i+j*4] * B[j];
      j++;
    } i++;
  }

}