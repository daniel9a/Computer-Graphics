
#ifndef __ECS175_Proj3__Coord__
#define __ECS175_Proj3__Coord__

#include <iostream>
#include <cmath>

class Coord{
 public: 
  Coord(){
    x=y=z=r=g=b=0;
  }
  float x, y, z, r, g, b;
  int corxy, corxz, coryz;
};

class Tri{
 public:
  Tri(){
    KA = KD = KS = n =1;
  }
  Coord *C1, *C2, *C3;
  int num1, num2, num3;
  int sort;
  int n;
  float normvec[3], KA, KD, KS;
  float plane;
  int renderfirst[100];
  int render;
  void findlimits(Tri *L, float *minx, float *maxx, float *miny, float *maxy, float *minz, float *maxz){

    *minx=*maxx = L->C1->x;

    *minx = (L->C2->x < *minx) ? L->C2->x : *minx;
    *maxx = (L->C2->x > *maxx) ? L->C2->x : *maxx;
    *minx = (L->C3->x < *minx) ? L->C3->x : *minx;
    *maxx = (L->C3->x > *maxx) ? L->C3->x : *maxx;

    *miny=*maxy = L->C1->y;
    
    *miny = (L->C2->y < *miny) ? L->C2->y : *miny;
    *maxy = (L->C2->y > *maxy) ? L->C2->y : *maxy;
    *miny = (L->C3->y < *miny) ? L->C3->y : *miny;
    *maxy = (L->C3->y > *maxy) ? L->C3->y : *maxy;

    *minz=*maxz = L->C1->z;

    *minz = (L->C2->z < *minz) ? L->C2->z : *minz;
    *maxz = (L->C2->z > *maxz) ? L->C2->z : *maxz;
    *minz = (L->C3->z < *minz) ? L->C3->z : *minz;
    *maxz = (L->C3->z > *maxz) ? L->C3->z : *maxz;

  }
  bool order(Tri *L){ 
    float minAX, minAY, minAZ, maxAX, maxAY, maxAZ, minBX, minBY, minBZ, maxBX, maxBY, maxBZ;
    findlimits(this, &minAX, &maxAX, &minAY, &maxAY, &minAZ, &maxAZ);
    findlimits(L, &minBX, &maxBX, &minBY, &maxBY, &minBZ, &maxBZ);

    float avgx = (C1->x + C2->x + C3->x)/3;
    float avgy = (C1->y + C2->y + C3->y)/3;
    float avgz = (C1->z + C2->z + C3->z)/3;
    float avgx2 = (L->C1->x + L->C2->x + L->C3->x)/3;
    float avgy2 = (L->C1->y + L->C2->y + L->C3->y)/3;
    float avgz2 = (L->C1->z + L->C2->z + L->C3->z)/3;

    if (sort==0){

      if (!(maxAX>=minBX))
	      return true;
      else if (!(minAX<=maxBX) || !(maxAY>=minBY) || !(minAY<=maxBY) || !(maxAZ>=minBZ) || !(minAZ<=maxBZ))
	      return false;

      float xx1 = (-(avgy * normvec[1] + avgz * normvec[2] + plane))/normvec[0];
      float xx2 = (-(avgy * L->normvec[1] + avgz * L->normvec[2] + L->plane))/L->normvec[0];
      float xx3 = (-(avgy2 * normvec[1] + avgz2 * normvec[2] + plane))/normvec[0];
      float xx4 = (-(avgy2 * L->normvec[1] + avgz2 * L->normvec[2] + L->plane))/L->normvec[0];
     
      return ((xx1 < xx2 && xx3 < xx4) ? true : false);
    }
    if (sort==1){
      if (!(maxAY>=minBY))
	      return true;
      else if (!(minAX<=maxBX) || !(maxAX>=minBX) || !(minAY<=maxBY) || !(maxAZ>=minBZ) || !(minAZ<=maxBZ))
	      return false;
     
      float yy1 = (-(avgx * normvec[0] + avgz * normvec[2] + plane))/normvec[1];
      float yy2 = (-(avgx * L->normvec[0] + avgz * L->normvec[2] + L->plane))/L->normvec[1];
      float yy3 = (-(avgx2 * normvec[0] + avgz2 * normvec[2] + plane))/normvec[1];
      float yy4 = (-(avgx2 * L->normvec[0] + avgz2 * L->normvec[2] + L->plane))/L->normvec[1];
   
      return ((yy1 < yy2 && yy3 < yy4) ? true : false);
    }
    else {
      if (!(maxAZ>=minBZ)){
	      return true;
      }
      else if (!(minAX<=maxBX) || !(maxAX>=minBX) || !(minAY<=maxBY) || !(maxAY>=minBY) || !(minAZ<=maxBZ)){
	      return false;
      }
   
      float zz1 = (-(avgx * normvec[0] + avgy * normvec[1] + plane))/normvec[2];
      float zz2 = (-(avgx * L->normvec[0] + avgy * L->normvec[1] + L->plane))/L->normvec[2];
      float zz3 = (-(avgx2 * normvec[0] + avgy2 * normvec[1] + plane))/normvec[2];
      float zz4 = (-(avgx2 * L->normvec[0] + avgy2 * L->normvec[1] + L->plane))/L->normvec[2];
     
      return ((zz1 < zz2 && zz3 < zz4) ? true : false);
      
    }
  }

};


#endif 
