
#include "Sort.h"
#include <iostream>
using namespace std;

/**
 * Construct the binary heap.
 * capacity is the capacity of the binary heap.
 */
TriHeap::TriHeap()
  : currentSize(0)
{
}

/**
 * Insert item x into the priority queue, maintaining heap order.
 * Duplicates are allowed.
 * Throw Overflow if container is full.
 */
void TriHeap::insert( Tri* x )
{

  // Percolate up
  array[currentSize]=x;
  returned[currentSize]=false;
  currentSize++;
}

bool TriHeap::isEmpty(){
  int i = 0;
  while (i < currentSize) {
  //for (int i=0; i<currentSize; i++){
    //if (!returned[i])
    if (returned[i] == false)
      return false;
    i++;
  }
  return true;
}

/**
 * Remove the smallest item from the priority queue.
 * Throw Underflow if empty.
 */
void TriHeap::sort(){
  int i = 0;
  while (i < currentSize) {
  //for(int i=0; i<currentSize; i++){
    returned[i]=false;
    array[i]->render=0;
    i++;
  }
  i = 0;
  while (i < currentSize) {
  //for(int i=0; i<currentSize; i++){
    int j = 0;
    while (j < currentSize) {
    //for (int j=0; j<currentSize; j++){
      if(i!=j){
        if (array[i]->order(array[j])){
          array[i]->renderfirst[array[i]->render]=j;
          array[i]->render+=1;
        }
      }
      j++;
    }
    i++;
  }
}

Tri* TriHeap::deleteMax( )
{
  bool found;
  
  for (int i=0; i<currentSize; i++){
    found=true;
    if (returned[i] == true)
      continue;
    for (int j=0; j<array[i]->render; j++){

      if(returned[array[i]->renderfirst[j]] == true){
        found=true; 
      }  
      else { 
        found=false;
        j+=100;
      }

    }
    if(found != false){
      returned[i]=true;
      return array[i];
    }
  }
  makeEmpty();
  return array[0];
}

void TriHeap::makeEmpty( )
{
  currentSize = 0;
}




////////////////////////////////////////////////////////////////////

void MakePixel(float *Buffer, int x, int y, float r, float g, float b)
{
  if (0 <= x && x <= 300 && 0 <= y && y <= 300)
  {
    int pixnum = x + y * 300;
    //R
    Buffer[0 + pixnum * 3] = r;
    //G
    Buffer[1 + pixnum * 3] = g;
    //B
    Buffer[2 + pixnum * 3] = b;
  }  
}


float findmax(float x, float y, float z, float extra)
{
  float temp;
  temp = (x > y) ? x : y;
  temp = (z > temp) ? z : temp;
  temp = (extra > temp) ? extra : temp;

  return temp;
}

float findmin(float x, float y, float z, float extra)
{
  float temp;
  temp = (x < y) ? x : y;
  temp = (z < temp) ? z : temp;
  temp = (extra < temp) ? extra : temp;

  return temp;
}


 
void matmult44Multiper(float A[], float B[], float C[])
{ //4x4 x 4x4 matrix multiplier

  int i = 0;
  while (i < 4) {
    int j = 0;
    while (j < 4) {
      C[i * 4 + j] = 0;
      int k = 0;
      while (k < 4) {
        C[i * 4 + j] += A[j + k * 4] * B[i * 4 + k];
        k++;
      } j++;
    } i++;
  }

}
 
void matmult41Multiper(float A[], float B[], float C[])
{ //4x4 x 4x1 matrix multiplier
  
  int i = 0;
  while (i < 4) {
    C[i] = 0;
    int k = 0;
    while (k < 4) {
      C[i] += A[i + k * 4] * B[k]; 
      k++;
    } i++;
  }
  
  
}

  
void Normalize(float *normvec)
{
  float length = pow(normvec[0] * normvec[0] + normvec[1] * normvec[1] + normvec[2] * normvec[2], .5);
  
  if(pow(normvec[0] * normvec[0] + normvec[1] * normvec[1] + normvec[2] * normvec[2], .5) != 0)
  
  { 
    normvec[0] /= length;
    normvec[1] /= length;
    normvec[2] /= length;

  }
}
 
float Dot(float *vec1, float *vec2) 
{
  return (vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]);
}

void setLight(Coord Light, float IA[])
{
  Light.r = (Light.r > 1) ? 1 : Light.r; 
  Light.r = (Light.r < 0) ? 0 : Light.r;
  Light.g = (Light.g > 1) ? 1 : Light.r;
  Light.g = (Light.g < 0) ? 0 : Light.g;
  Light.b = (Light.b > 1) ? 1 : Light.b;
  Light.b = (Light.b < 0) ? 0 : Light.b;

  IA[0] = (IA[0] > 1) ? 1 : IA[0];
  IA[0] = (IA[0] < 0) ? 0 : IA[0];
  IA[1] = (IA[1] > 1) ? 1 : IA[1];
  IA[1] = (IA[1] < 0) ? 0 : IA[1];
  IA[2] = (IA[2] > 1) ? 1 : IA[2];
  IA[2] = (IA[2] < 0) ? 0 : IA[2]; 
}

void Phong(float *vertcol, Tri *L, Coord Light, float IA[3], float Viewer[3])   
{
  float lightvec[3] = {Light.x - L->C1->x, Light.y - L->C1->y, Light.z - L->C1->z};
  float viewvec[3] = {Viewer[0] - L->C1->x, Viewer[1] - L->C1->y, Viewer[2] - L->C1->z};
  float temp1 = pow(viewvec[0] * viewvec[0] + viewvec[1] * viewvec[1] + viewvec[2] * viewvec[2], .5);
  float temp2 = pow(lightvec[0] * lightvec[0] + lightvec[1] * lightvec[1] + lightvec[2] * lightvec[2], .5);

  float length = temp1 + temp2;
  Normalize(lightvec);
  Normalize(viewvec);

  float Reflectvec[3] = {2 * (Dot(L->normvec, lightvec) * L->normvec[0]) - lightvec[0], 2 * (Dot(L->normvec, lightvec) * L->normvec[1]) - lightvec[1], 2 * (Dot(L->normvec, lightvec) * L->normvec[2]) - lightvec[2]};
  Normalize(Reflectvec);

  vertcol[8] = ((L->KD * Dot(lightvec, L->normvec)) + L->KS * pow(abs(Dot(Reflectvec, viewvec)), L->n)) / length;
  for (int i = 0; i < 3; i++) {
    if (i == 0) {vertcol[i] = L->C1->r * (Light.r * vertcol[8] + IA[i] * L->KA); }
    else if (i == 1) { vertcol[i] = L->C1->g * (Light.g * vertcol[8] + IA[i] * L->KA); }
    else if (i == 2) { vertcol[i] = L->C1->b * (Light.b * vertcol[8] + IA[i] * L->KA); }
  }

  for (int i = 0; i < 3; i++) {
    if (i == 0) {
      lightvec[i] = Light.x - L->C2->x;
      viewvec[i] = Viewer[i] - L->C2->x;
    } else if (i == 1) {
      lightvec[i] = Light.y - L->C2->y;
      viewvec[i] = Viewer[i] - L->C2->y;
    } else if (i == 2) {
      lightvec[i] = Light.z - L->C2->z;
      viewvec[i] = Viewer[i] - L->C2->z;
    }
  }

  Normalize(lightvec);
  Normalize(viewvec);
  for (int i = 0; i < 3; i++) { Reflectvec[i] = 2 * (Dot(L->normvec, lightvec) * L->normvec[i]) - lightvec[i]; }


  Normalize(Reflectvec);
  vertcol[8] = ((L->KD * Dot(lightvec, L->normvec)) + L->KS * pow(abs(Dot(Reflectvec, viewvec)), L->n)) / length;
  for (int i = 0; i < 3; i++) {
    if (i == 0) {vertcol[i+3] = L->C2->r * (Light.r * vertcol[8] + IA[i] * L->KA); }
    else if (i == 1) { vertcol[i+3] = L->C2->g * (Light.g * vertcol[8] + IA[i] * L->KA); }
    else if (i == 2) { vertcol[i+3] = L->C2->b * (Light.b * vertcol[8] + IA[i] * L->KA); }
  }

  for (int i = 0; i < 3; i++) {
    if (i == 0) {
      lightvec[i] = Light.x - L->C2->x;
      viewvec[i] = Viewer[i] - L->C2->x;
    } else if (i == 1) {
      lightvec[i] = Light.y - L->C2->y;
      viewvec[i] = Viewer[i] - L->C2->y;
    } else if (i == 2) {
      lightvec[i] = Light.z - L->C2->z;
      viewvec[i] = Viewer[i] - L->C2->z;
    }
  }


  Normalize(lightvec);
  Normalize(viewvec);

  for (int i = 0; i < 3; i++) { Reflectvec[i] = 2 * (Dot(L->normvec, lightvec) * L->normvec[i]) - lightvec[i]; }


  Normalize(Reflectvec);
  vertcol[8] = ((L->KD * Dot(lightvec, L->normvec)) + L->KS * pow(abs(Dot(Reflectvec, viewvec)), L->n)) / length;
  for (int i = 0; i < 3; i++) {
    if (i == 0) {vertcol[i+6] = L->C3->r * (Light.r * vertcol[8] + IA[i] * L->KA); }
    else if (i == 1) { vertcol[i+6] = L->C3->g * (Light.g * vertcol[8] + IA[i] * L->KA); }
    else if (i == 2) { vertcol[i+6] = L->C3->b * (Light.b * vertcol[8] + IA[i] * L->KA); }
  }

}




void Translate(int i, float DeltaX, float DeltaY, float DeltaZ, float MasterMax, float MasterMin, int sign, Coord *Coordinates[5][20], int numTri[6][2])
{
  int j = 0;
  while (j < numTri[i][0]) {
    Coordinates[i][j]->x += DeltaX * sign * (MasterMax - MasterMin) / 100; //Translation function
    Coordinates[i][j]->y += DeltaY * sign * (MasterMax - MasterMin) / 100;
    Coordinates[i][j]->z += DeltaZ * sign * (MasterMax - MasterMin) / 100;
    j++;
  }
} 


void Halftone(float *Buffer) 
{
  float avg;

  int i = 0;
  while (i < 270000) {
    int j = 0;
    while (j < 900) {
      avg = 0;
      int a = i;
      while (a < i + 2700) {
        int b = j;
        while (b < j + 9) {
          avg += Buffer[a + b];
          b++;
        }
        a += 900;
      }
      avg = avg / 27;
      if (avg < 0.1)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 0;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 0;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 0;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 0;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 0;
      }
      else if (avg < 0.2)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 0;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 0;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 0;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 0;
      }
      else if (avg < 0.3)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 0;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 0;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 1;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 0;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 1;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 0;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 0;
      }
      else if (avg < 0.4)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 0;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 0;
      }
      else if (avg < 0.5)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 0;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }
      else if (avg < 0.6)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 0;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }
      else if (avg < 0.7)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 1;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 0;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }
      else if (avg < 0.8)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 1;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 0;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 0;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 1;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }
      else if (avg < 0.9)
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 1;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 1;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 0;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 1;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 1;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }
      else
      {
        Buffer[i + j] = Buffer[i + j + 1] = Buffer[i + j + 2] = 1;
        Buffer[i + j + 3] = Buffer[i + j + 4] = Buffer[i + j + 5] = 1;
        Buffer[i + j + 6] = Buffer[i + j + 7] = Buffer[i + j + 8] = 1;
        Buffer[i + j + 900] = Buffer[i + j + 901] = Buffer[i + j + 902] = 1;
        Buffer[i + j + 903] = Buffer[i + j + 904] = Buffer[i + j + 905] = 1;
        Buffer[i + j + 906] = Buffer[i + j + 907] = Buffer[i + j + 908] = 1;
        Buffer[i + j + 1800] = Buffer[i + j + 1801] = Buffer[i + j + 1902] = 1;
        Buffer[i + j + 1803] = Buffer[i + j + 1804] = Buffer[i + j + 1805] = 1;
        Buffer[i + j + 1806] = Buffer[i + j + 1807] = Buffer[i + j + 1808] = 1;
      }

      j += 9;
    }

    i += 2700; 
  }
}


void Rotate(int i, float DeltaX, float DeltaY, float DeltaZ, int numTri[6][2], Coord *Coordinates[5][20], int sign)
{ //Rotation Function
  float theta = .03;
  if (DeltaX || DeltaY || DeltaZ)
  {
    float M[16];
    float avg = 0;
    int n;

    for (n = 0; n < numTri[i][0]; n++)
    {
      avg += Coordinates[i][n]->x;
    }
    avg /= n;
    
    if (DeltaX)
    {

      float M1[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -avg, 0, 0, 1};                                                                             //First Translation Matrix
      float M3[] = {1, 0, 0, 0, 0, float(cos(theta)), float(sin(theta * sign)), 0, 0, float(-sin(theta * sign)), float(cos(theta)), 0, 0, 0, 0, 1}; //Rotate about X
      float M5[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, avg, 0, 0, 1};                                                                              //Translate back to normal
      matmult44Multiper(M5, M3, M);
      matmult44Multiper(M, M1, M5);
      int j = 0;
      while (j < numTri[i][0]) {

        float in_vert[] = {Coordinates[i][j]->x, Coordinates[i][j]->y, Coordinates[i][j]->z, 1};
        float out_vert[4];
        matmult41Multiper(M5, in_vert, out_vert); //Apply rotation matrix to vertex
        Coordinates[i][j]->x = out_vert[0];
        Coordinates[i][j]->y = out_vert[1];
        Coordinates[i][j]->z = out_vert[2];
        j++;
      }
    }
    else if (DeltaZ)
    {

      float M1[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -avg, 1};                                                                             //First Translation Matrix
      float M3[] = {float(cos(theta)), float(sin(theta * sign)), 0, 0, float(-sin(theta * sign)), float(cos(theta)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; //Rotate about Z
      float M5[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, avg, 1};                                                                              //Translate back to normal
      matmult44Multiper(M5, M3, M);
      matmult44Multiper(M, M1, M5);
      int j = 0;
      while (j < numTri[i][0]) {

        float in_vert[] = {Coordinates[i][j]->x, Coordinates[i][j]->y, Coordinates[i][j]->z, 1};
        float out_vert[4];
        matmult41Multiper(M5, in_vert, out_vert); //Apply rotation matrix to vertex
        Coordinates[i][j]->x = out_vert[0];
        Coordinates[i][j]->y = out_vert[1];
        Coordinates[i][j]->z = out_vert[2];
        j++;
      }
    }
    else if (DeltaY)
    {

      float M1[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -avg, 0, 1};                                                                             //First Translation Matrix
      float M3[] = {float(cos(theta)), 0, float(-sin(theta * sign)), 0, 0, 1, 0, 0, float(sin(theta * sign)), 0, float(cos(theta)), 0, 0, 0, 0, 1}; //Rotate about X
      float M5[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, avg, 0, 1};                                                                              //Translate back to normal
      matmult44Multiper(M5, M3, M); 
      matmult44Multiper(M, M1, M5);
      int j = 0;
      while (j < numTri[i][0]) {

        float in_vert[] = {Coordinates[i][j]->x, Coordinates[i][j]->y, Coordinates[i][j]->z, 1};
        float out_vert[4];
        matmult41Multiper(M5, in_vert, out_vert); //Apply rotation matrix to vertex
        Coordinates[i][j]->x = out_vert[0];
        Coordinates[i][j]->y = out_vert[1];
        Coordinates[i][j]->z = out_vert[2];
        j++;
      }
    }



    
    
  }
}

