#include "Raytrace.h"
#include <cmath>
#include <iostream>

using namespace std;


float normalize(float X[3]){
    float distance = X[0]*X[0] + X[1]*X[1] + X[2]*X[2];
    distance = pow(distance, .5);

    if (distance!=0) {
        for (int i = 0; i < 3; i++) { X[i] = X[i]/distance; }
    } else {
        X[0] = X[1] = 0;
        X[2] = 1;
    }
    return distance; 
}

void cross(float X[3], float Y[3], float Z[3]){
    Z[0] = X[1]*Y[2] - Y[1]*X[2];
    Z[1] = X[2]*Y[0] - Y[2]*X[0];
    Z[2] = X[0]*Y[1] - Y[0]*X[1];
}

float dot(float X[3], float Y[3]){ return Y[0]*X[0] + Y[1]*X[1] + Y[2]*X[2]; }

light::light(float *X){
    for (int i=0; i < 3; i++) {
        Coordinate[i] = X[i];
        Color[i] = X[i+3];
    }
}

sphere::sphere(float *X){
    for(int i = 0; i < 3; i++) {
        Coordinate[i] = X[i];
        Ellipse[i] = X[i+3];
    }
}

void sphere::KAdd(float *X){
    for (int i=0; i < 10; i++) { K[i] = X[i]; }
}

float sphere::NearHit(float Source[3], float Ray[3]){
    float P1[3],V1[3], A, B, C;

    for (int i = 0; i < 3; i++) {
        P1[i] = Source[i] * Ellipse[i] - Coordinate[i] * Ellipse[i];
        V1[i] = Ray[i] * Ellipse[i];
    }

    A = dot(V1, V1);
    B = 2 * dot(V1, P1);
    C = dot(P1, P1) - 1;
    P1[2]=B*B-4*A*C;

    if((P1[2])>0){
        P1[1] =(pow(P1[2],.5));
        P1[0] = ((-B) + P1[1])/(2*A);
        P1[1] = ((-B) - P1[1])/(2*A);
        if ((P1[0]>0.03) &&  ((P1[0]<P1[1])||(P1[1]<=0.03))){ return P1[0]; }
        else if(P1[1]>.03) { return P1[1]; }
        else { return -1; }
    }
    else return -1;
}

void sphere::getSource(float Ray[3], float KValue[10], float newSource[3], float newNormal[3]){
    for(int i = 0; i < 3; i++) {
        newNormal[i] = 2 * (newSource[i]-Coordinate[i]) * Ellipse[i]*Ellipse[i];
    }
    if (dot(newNormal,Ray)>0){
        for (int i = 0 ; i < 3; i++) {
            newNormal[i] = -newNormal[i];
        }
    }
    for (int i = 0 ; i < 10; i++) { KValue[i] = K[i]; }
}



plane::plane(float *X){
    for(int i =0; i < 3; i++) { ABC[i] = X[i]; }
    D = X[3];
}

void plane::AddPoints(float *X){
    for (int i = 0; i < 3; i++) {
        Point1[i] = X[i];
        Point2[i] = X[i+3];
        Point3[i] = X[i+6];
    }

    for (int i=0; i<3; i++){
        AB[i] = Point2[i] - Point1[i];
        BC[i] = Point3[i] - Point2[i];
        CA[i] = Point1[i] - Point3[i];
    }
}

void plane::KAdd(float *X){
    for (int i = 0; i < 10; i++) {
        K[i] = X[i];
    }
}

float plane::NearHit(float Source[3], float Ray[3]){
    float x = dot(ABC, Ray);
    //if (x==0){ return -1; }

    if (x!=0) {
        float testcross[3], intersection[3], BI[3], CI[3];
        float t = (D - dot(ABC,Source))/x;
        
        for (int i = 0; i < 3; i++) {
            intersection[i] = Source[i] + t*Ray[i];
            BI[i] = intersection[i] - Point2[i];
            CI[i] = intersection[i] - Point3[i];
        }
        cross(AB, BI, testcross);
        if ( dot(testcross,ABC)>0){
            cross(BC, CI, testcross);
            if ( dot(testcross,ABC)>0){
                float AI[3];

                for (int i=0; i < 3; i++) {
                    AI[i] = intersection[i] - Point1[i];
                }

                cross(CA, AI, testcross);
                if (dot(testcross,ABC)>0){
                    return t;
                }
                else return -1;
            }
            else return -1;
        }
        else{
            cross(BC, CI, testcross);
            if ( dot(testcross,ABC)<0){
                float AI[3];
                
               for (int i=0; i < 3; i++) {
                    AI[i] = intersection[i] - Point1[i];
                }
                cross(CA, AI, testcross);
                if (dot(testcross,ABC)<0){
                    return t;
                }
                else return -1;
            }
            else return -1;
        }
    } else {return -1;}
}

void plane::getSource(float OldRay[3], float KValue[10], float newNormal[3]){

    if (dot(OldRay,ABC)>0) {
        for (int i = 0; i < 3; i++) {
            newNormal[i] = -ABC[i];
        }
    } else {
        for (int i = 0; i < 3; i++) {
            newNormal[i] = ABC[i];
        }
    }


    for (int i = 0; i < 10; i++) {
        KValue[i] = K[i];
    }

}
