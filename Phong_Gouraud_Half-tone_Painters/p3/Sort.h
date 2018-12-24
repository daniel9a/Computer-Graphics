

#ifndef __ECS175_Proj3__Sort__
#define __ECS175_Proj3__Sort__

#include <stdio.h>
#include "Coord.h"



class TriHeap
{
 public:
  explicit TriHeap();
  bool isEmpty();
  void insert( Tri *x );
  Tri* deleteMax( );
  void makeEmpty( );
  void sort();

 private:
  int currentSize;  // Number of elements in heap
  Tri* array[100];// The heap array
  bool returned[100];
};

float findmax(float x, float y, float z, float extra);
float findmin(float x, float y, float z, float extra);

//void Translate(int i, float DeltaX, float DeltaY, float DeltaZ, float max, float min, int sign, Coord *Coordinates, int *numTri);

void matmult44Multiper(float A[], float B[], float C[]);
void matmult41Multiper(float A[], float B[], float C[]);

float Dist(float *vec);
void Normalize(float *normvec);
float Dot(float *vec1, float *vec2);
void setLight(Coord Light, float IA[]);
void Phong(float *vertcol, Tri *L, Coord Light, float IA[3], float Viewer[3]);    
void Halftone(float *Buffer);
void Translate(int i, float DeltaX, float DeltaY, float DeltaZ, float MasterMax, float MasterMin, int sign, Coord *Coordinates[5][20], int numTri[6][2]);


void Rotate(int i, float DeltaX, float DeltaY, float DeltaZ, int numTri[6][2], Coord *Coordinates[5][20], int sign);



#endif /* defined(__ECS175_Proj3__Sort__) */
