
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "shape.h"
#include <fstream>
#include <sstream> 
#include <cmath>
#include <stdio.h>
#include <string.h>
using namespace std;

void draw();
Poly S1, S2, S3, S4, S5, *S;
int numshape;
int MainWindow, Window3, Window1, Window2;
int coordinates[2*3][50][2];
int Vec_id, Tran_id, Scal_id, Rot_id, Menu_id, Cor_id, File_id, sign=1;
int menuValue=31, oldvalue=31;
float Maximum, Minimum, scalfac, deltZ, deltY, deltX;
Pt p1;
Pt p2;
//string filestring = "3dshape"; //Initial File Name
string filestring = "test.txt"; 

void check(int i) {
  switch (i) {
    case 0: S=&S1;
      break;
    case 1: S=&S2;
      break;
    case 2: S=&S3;
      break;
    case 3: S=&S4;
      break;
    case 4: S=&S5;
      break;
  }
}

void showCoord(int num, float x, float y, float z) {
  check(num);
  cout << "Shape " << num+1 << ":\n";
  for (int i=0; i<S->numV;i++){
    S->findvert(&x, &y, &z, i);
    cout << "Vertex " << i+1 << ":    X: " << x << " Y: " << y << " Z: " << z << "\n";
  }
}



void load(){ //Load function, reads in data from 3dcoordinates.txt
  string line;
  int n,a,b;
  float x,y,z;
  ifstream myfile(filestring);
  
  if (!myfile.is_open()) {
    cout << "Couldn't Find File Exited\n";
    exit(0);
  }
  else { 
    getline(myfile,line);
    numshape=stoi(line);
    int i = 0;
    while (i < numshape) {
      check(i); 
      getline(myfile,line);
      n=stoi(line);
      int j = 0;
      while (j < n) {
        getline(myfile,line);
        size_t pos = line.find(" ");
        x = stof(line.substr(0,pos));
        line.erase(0 , pos+1);
        pos = line.find(" ");
        y = stof(line.substr(0,pos));
        line.erase(0, pos+1);
        z = stof(line);
        S->addvert(x, y, z);
        j++;
      } 
      getline(myfile,line);
      n=stoi(line);
      coordinates[5][i][0]=n;
      int k = 0;
      while (k < n) {
        getline(myfile,line);
        size_t pos = line.find(" ");
        a = stoi(line.substr(0,pos))-1;
        line.erase(0 , pos+1);
        b = stoi(line)-1;
        coordinates[i][k][0]=a;
        coordinates[i][k][1]=b;
        k++;
      } i++;
    }
    /////////////////////////////////////////////////////////////////
    getline(myfile,line);
    size_t pos = line.find(" ");
    p1.x = stof(line.substr(0,pos));
    line.erase(0 , pos+1);
    pos = line.find(" ");
    p1.y = stof(line.substr(0,pos));
    line.erase(0, pos+1);
    p1.z = stof(line);
    getline(myfile,line);
    pos = line.find(" ");
    p2.x = stof(line.substr(0,pos));
    line.erase(0,pos+1);
    pos = line.find(" ");
    p2.y = stof(line.substr(0,pos));
    line.erase(0,pos+1);
    p2.z = stof(line);
  } 
}


void save(){ // Save the data to 3dcoordinates.txt , runs before quitting
  int n;
  float x,y,z;
  ofstream myfile (filestring);
  if (myfile.is_open()){
    myfile<<numshape<<"\n";
    int i=0;
    while(i < numshape) {
      check(i);
      n = S->numV;
      myfile << n << "\n";
      int j=0;
      while(j<n){
        S->findvert(&x,&y,&z,j);
        myfile<<x<<" "<<y<<" "<<z<<"\n";
        j++;
      }
      n= coordinates[5][i][0];
      myfile << n << "\n"; 
	    int k = 0;
      while (k < n) {
        myfile << (coordinates[i][k][0]+1) << " " << (coordinates[i][k][1]+1) << "\n";
        k++;
      } i++;
    }

    myfile << p1.x << " " << p1.y << " " << p1.z << "\n";
    myfile << p2.x << " " << p2.y << " " << p2.z << "\n";
    myfile.close();
  }
}

void menu(int num){
  if(num == 0){
    save();
    glutDestroyWindow(MainWindow);
    exit(0);
  } else if (num>40){ menuValue=num; } 
  else {
    oldvalue = num;
    menuValue = num;
  }
  glutPostRedisplay();
}

void createmenu(){ //All possible menu options

  Vec_id = glutCreateMenu(menu);
  glutAddMenuEntry("X1 ; Y1 ; Z1", 1);
  glutAddMenuEntry("X2 ; Y2 ; Z2", 2);
  Scal_id = glutCreateMenu(menu);
  glutAddMenuEntry("Poly 1", 11);
  glutAddMenuEntry("Poly 2", 12);
  glutAddMenuEntry("Poly 3", 13);
  glutAddMenuEntry("Poly 4", 14);
  glutAddMenuEntry("Poly 5", 15);
  Tran_id = glutCreateMenu(menu);
  glutAddMenuEntry("Poly 1", 21);
  glutAddMenuEntry("Poly 2", 22);
  glutAddMenuEntry("Poly 3", 23);
  glutAddMenuEntry("Poly 4", 24);
  glutAddMenuEntry("Poly 5", 25);
  Rot_id = glutCreateMenu(menu);
  glutAddMenuEntry("Poly 1", 31);
  glutAddMenuEntry("Poly 2", 32);
  glutAddMenuEntry("Poly 3", 33);
  glutAddMenuEntry("Poly 4", 34);
  glutAddMenuEntry("Poly 5", 35);
  Cor_id = glutCreateMenu(menu);
  glutAddMenuEntry("Poly 1", 41);
  glutAddMenuEntry("Poly 2", 42);
  glutAddMenuEntry("Poly 3", 43);
  glutAddMenuEntry("Poly 4", 44);
  glutAddMenuEntry("Poly 5", 45);
  glutAddMenuEntry("Rotation Vector", 46);
  Menu_id = glutCreateMenu(menu);
  glutAddSubMenu("Rotate", Rot_id);
  glutAddSubMenu("Translate", Tran_id);
  glutAddSubMenu("Scale", Scal_id);
  glutAddSubMenu("Rotation Vector", Vec_id);
  glutAddSubMenu("Print Vertex", Cor_id);
  glutAddMenuEntry("New File",50);
  glutAddMenuEntry("Exit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display1(){ //This is the subwindow for XY
  glutSetWindow(Window3); //Display2 and Display3 are essentially the same (except for XZ, YZ)
  glClear(GL_COLOR_BUFFER_BIT);
  float x1, x2, y1, y2, z;
  glBegin(GL_LINES); //Draw rotation vector
  glColor3f(0.0, 1.0, 0.0);
  glVertex2f((p1.x-Minimum)*scalfac-.99, (p1.y-Minimum)*scalfac-.99);
  glColor3f(0.0, 1.0, 0.0);
  glVertex2f((p2.x-Minimum)*scalfac-.99, (p2.y-Minimum)*scalfac-.99);
  glEnd();
  glColor3f(.5, 0.0, 0.0);
  int i=0;
  while(i<numshape){
    glColor3f(1.0, 0.2*float(i), 0.0);
    check(i); 
    int j=0;
    while(j<coordinates[6-1][i][0]){
      S->findvert(&x1, &y1, &z, coordinates[i][j][0]);
      S->findvert(&x2, &y2, &z, coordinates[i][j][1]);
      glBegin(GL_LINES); //Draw all the lines
      glVertex2f((x1-Minimum)*scalfac-.99, (y1-Minimum)*scalfac-.99);
      glVertex2f((x2-Minimum)*scalfac-.99, (y2-Minimum)*scalfac-.99);
      glEnd();
      j++;
    } i++;
  }
  glutSwapBuffers();
}

void display2(){
  glutSetWindow(Window1);
  glClear(GL_COLOR_BUFFER_BIT);
  float x1, x2, y, z1, z2;
  glBegin(GL_LINES);
  glColor3f(0.0, 0.0, 1.0);
  glVertex2f((p1.x-Minimum)*scalfac-.99, (p1.z-Minimum)*scalfac-.99);
  glColor3f(0.0, 0.0, 1.0);
  glVertex2f((p2.x-Minimum)*scalfac-.99, (p2.z-Minimum)*scalfac-.99);
  glEnd();
  int i=0;
  while(i<numshape){
    glColor3f(0.0, 1.0, 0.2*float (i));
    check(i);
    int j=0;
    while(j<coordinates[6-1][i][0]){
      S->findvert(&x1, &y, &z1, coordinates[i][j][0]);
      S->findvert(&x2, &y, &z2, coordinates[i][j][1]);
      glBegin(GL_LINES);
      glVertex2f((x1-Minimum)*scalfac-.99, (z1-Minimum)*scalfac-.99);
      glVertex2f((x2-Minimum)*scalfac-.99, (z2-Minimum)*scalfac-.99);
      glEnd();
      j++;
    } i++;
  }
  glutSwapBuffers();
}

void display3(){
  glutSetWindow(Window2);
  glClear(GL_COLOR_BUFFER_BIT);
  float x, y1, y2, z1, z2;
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex2f((p1.y-Minimum)*scalfac-.99, (p1.z-Minimum)*scalfac-.99);
  glColor3f(1.0, 0.0, 0.0);
  glVertex2f((p2.y-Minimum)*scalfac-.99, (p1.z-Minimum)*scalfac-.99);
  glEnd();

  int i=0;
  while(i<numshape){
    glColor3f(0.2*float (i), 0.0, 1.0);
    check(i);
    int j=0;
    while(j<coordinates[6-1][i][0]){
      S->findvert(&x, &y1, &z1, coordinates[i][j][0]);
      S->findvert(&x, &y2, &z2, coordinates[i][j][1]);
      glBegin(GL_LINES);
      glVertex2f((y1-Minimum)*scalfac-.99, (z1-Minimum)*scalfac-.99); //try 
      glVertex2f((y2-Minimum)*scalfac-.99, (z2-Minimum)*scalfac-.99);
      glEnd();
      j++;
    } i++;
  }
  glutSwapBuffers();
}

void drawall(){ //Main Draw function, called every update
  int num = menuValue%10-1;
  float x,y,z;
  float delta = (Maximum-Minimum)*200; 
  float theta = -deltX*sign/200;
  switch(menuValue) {
    case 0:
      save();
      glutDestroyWindow(MainWindow);
      exit(0);
      break;
    case 1:
      p1.x+=deltX/delta;
      p1.y+=deltY/delta;
      p1.z+=deltZ/delta;
      break;
    case 2:
      p2.x+=deltX/delta;
      p2.y+=deltY/delta;
      p2.z+=deltZ/delta;
      break;
    case 11:
      if (deltZ!=0)
        S1.scale(1+deltZ*sign/200);
      else if (deltX!=0)
        S1.scale(1-deltX*sign/200);
      break;
    case 12:
      if (deltZ!=0)
        S2.scale(1+deltZ*sign/200);
      else if (deltX!=0)
        S2.scale(1-deltX*sign/200);
      break;
    case 13:
      if (deltZ!=0)
        S3.scale(1+deltZ*sign/200);
      else if (deltX!=0)
        S3.scale(1-deltX*sign/200);
      break;
    case 14:
      if (deltZ!=0)
        S4.scale(1+deltZ*sign/200);
      else if (deltX!=0)
        S4.scale(1-deltX*sign/200);
      break;
    case 15:
      if (deltZ!=0)
        S5.scale(1+deltZ*sign/200);
      else if (deltX!=0)
        S5.scale(1-deltX*sign/200);
      break;
    case 21:
      S1.translate(deltX/delta, deltY/delta, deltZ/delta);
      break;
    case 22:
      S2.translate(deltX/delta, deltY/delta, deltZ/delta);
      break;
    case 23:
      S3.translate(deltX/delta, deltY/delta, deltZ/delta);
      break;
    case 24:
      S4.translate(deltX/delta, deltY/delta, deltZ/delta);
      break;
    case 25:
      S5.translate(deltX/delta, deltY/delta, deltZ/delta);
      break;
    case 31:
      if (deltZ!=0)
        S1.rotate(p1, p2, deltZ*sign/200);
      else if (deltX!=0)
        S1.rotate(p1, p2, theta);
      break;
    case 32:
      if (deltZ!=0)
        S2.rotate(p1, p2, deltZ*sign/200);
      else if (deltX!=0)
        S2.rotate(p1, p2, theta);
      break;
    case 33:
      if (deltZ!=0)
        S3.rotate(p1, p2, deltZ*sign/200);
      else if (deltX!=0)
        S3.rotate(p1, p2, theta);
      break;
    case 34:
      if (deltZ!=0)
        S4.rotate(p1, p2, deltZ*sign/200);
      else if (deltX!=0)
        S4.rotate(p1, p2, theta);
      break;
    case 35:
      if (deltZ!=0)
        S5.rotate(p1, p2, deltZ*sign/200);
      else if (deltX!=0)
        S5.rotate(p1, p2, theta);
      break;
    case 41:
      showCoord(num, x, y, z);
      break;
    case 42:
      showCoord(num, x, y, z);
      break;
    case 43:
      showCoord(num, x, y, z);
      break;
    case 44:
      showCoord(num, x, y, z);
      break;
    case 45:
      showCoord(num, x, y, z);
      break;
    case 46:
      cout<< "Vector Coordinate 1 : X: "<< p1.x << " Y: " << p1.y << " Z: " << p1.z << "\n";
      cout<< "Vector Coordinate 2 : X: "<< p2.x << " Y: " << p2.y << " Z: " << p2.z << "\n";
      break;
    case 50:
      cout << "Enter Name of New Text File:";
      cin >> filestring;
      load();
      menuValue=oldvalue;
      break;
  }

  //for (int i=0; i<numshape;i++){
  int i=0;
  while(i<numshape){
    check(i);
    if (S->getMax()>Maximum) { Maximum = S->getMax(); }
    if (S->getMin()<Minimum) { Minimum = S->getMin(); }
    i++;
  }
  scalfac =2/(Maximum-Minimum);
  display3();
  display2();
  display1(); //Display each shape
  draw();
  if((deltZ!=0) || (deltY!=0) || (deltX!=0)){
    glutSetWindow(MainWindow);
    glutPostRedisplay();
  }
}

void pressKey(int key, int x, int y) {
  if (key == GLUT_KEY_UP) {
    deltZ = 10*sign;
  } else if (key == GLUT_KEY_DOWN) {
    deltX = 10*sign;
  } else if (key == GLUT_KEY_RIGHT) {
    deltY = 10*sign;
  }

  glutSetWindow(MainWindow);
  glutPostRedisplay();
}

void releaseKey(int key, int x, int y) {
if (key == GLUT_KEY_UP) {
    deltZ = 0;
  } else if (key == GLUT_KEY_DOWN) {
    deltX = 0;
  } else if (key == GLUT_KEY_RIGHT) {
    deltY = 0;
  } else if (key == GLUT_KEY_LEFT) {
    sign*=-1;
  }

  glutSetWindow(MainWindow);
  glutPostRedisplay();
}

void draw() {                          //main window isnt actually used
  glutSetWindow(MainWindow);
  glClear(GL_COLOR_BUFFER_BIT);//clears the colour and depth buffers
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2f(0.4,-.35); 
  if (sign==-1)
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"LEFT (Negate)");
  glRasterPos2f(0.4,-.1);
  if (!(deltZ==0))
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"UP (move Z)");
  glRasterPos2f(0.4,-.65);
  if (!(deltY==0))
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"RIGHT (move Y)");
  glRasterPos2f(0.4,-.9);
  if (!(deltX==0))
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"DOWN (move X)");
  glutSwapBuffers();

}   

int main(int argc, char **argv) {
  
  load();

  glutInit(&argc,argv);                                   
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  MainWindow = glutCreateWindow("Proj2");
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  createmenu();
  glutDisplayFunc(drawall);
  Window3 = glutCreateSubWindow(MainWindow, 0, 250, 250, 250);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display1);

  Window1 = glutCreateSubWindow(MainWindow, 0, 0, 250, 250);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display2);

  Window2 = glutCreateSubWindow(MainWindow, 250, 0, 250, 250);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display3);
  glutMainLoop();           
}
                
