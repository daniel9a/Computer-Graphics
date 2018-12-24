#ifndef __Raytrace__
#define __Raytrace__

#include <stdio.h>

class light{
public:
    light(float *X);
    float Coordinate[3], Color[3];
}; 
 
class sphere{ 
public:
    sphere(float *X);
    float Coordinate[3], Ellipse[3], K[10];
    void KAdd(float *X);
    float NearHit(float Source[3], float Ray[3]);
    void getSource(float Ray[3], float KValue[10], float newSource[3], float newNormal[3]);
};

class plane{ 
public:
    plane(float *X);
    float ABC[3] , D, K[10], AB[3], BC[3], CA[3];
    float Point1[3], Point2[3], Point3[3];
    void AddPoints(float *X);
    void KAdd(float *X);
    float NearHit(float Source[3], float Ray[3]);
    void getSource(float OldRay[3], float KValue[10], float newNormal[3]);
};

float dot(float X[3], float Y[3]);
void cross(float X[3], float Y[3], float Z[3]);
float normalize(float X[3]);
//void findreflection(float Ray[3], float Normal[3], float Reflection[3]);


#endif