
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream>
#include <cstring> 
#include "Coord.h"
#include "Sort.h" 
//#include "Shape.h"
#include <cmath>
using namespace std;
 

int MainWindow, xyWindow, xzWindow, yzWindow;
Coord *Coordinates[5][20]; // Shape, coordinate (x,y,z)+(r,g,b)
Tri *Lines[5][20];         // i = Shape, j = Triangles in shape
int numTri[6][2];          // i = Shape, j=0 = # of coordinates, j=1 = # of triangles
string filestring = "test.txt"; 
float PixelBuffer1[300 * 300 * 3], PixelBuffer2[300 * 300 * 3], PixelBuffer3[300 * 300 * 3];
int numshape, lineloc[300][2];
float Minimum, Maximum, scalfac, DeltaX = 0, DeltaY = 0, DeltaZ = 0;
TriHeap PaintOrder;
bool halftoneCheck = false;
Coord Light; 
float IA[3], Viewer[3];
int menuValue = 24, Menu_id, Phong_id, Light_id, View_id, Move_id,  sign = 1;
//int S1, S2, S3, S4, S5, men1, men2, men3, men4, men5,

void load()
{
  string line;
  Coord *C;
  Tri *L;
  int n, num;
  ifstream myfile(filestring);
  if (myfile.is_open())
  {
    getline(myfile, line);
    numshape = stoi(line);
    int i = 0;
    while (i < numshape) {
      getline(myfile, line);
      n = stoi(line);
      numTri[i][0] = n;
      int j = 0;
      while (j < n) {
        C = new Coord();
        getline(myfile, line);
        size_t pos = line.find(" ");
        C->x = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(" ");
        C->y = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(" ");
        C->z = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(" ");
        C->r = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(" ");
        C->g = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        C->b = stof(line);
        Coordinates[i][j] = C;
        j++;
      }
      getline(myfile, line);
      n = stoi(line);
      numTri[i][1] = n;
      j = 0;
      while (j < n) {
        L = new Tri();
        getline(myfile, line);
        size_t pos = line.find(" ");
        num = stoi(line.substr(0, pos));
        L->C1 = Coordinates[i][num - 1];
        L->num1 = num;
        line.erase(0, pos + 1);
        pos = line.find(" ");
        num = stoi(line.substr(0, pos));
        L->C2 = Coordinates[i][num - 1];
        L->num2 = num;
        line.erase(0, pos + 1);
        num = stoi(line);
        L->C3 = Coordinates[i][num - 1];
        L->num3 = num;
        Lines[i][j] = L;
        j++;
      }
      i++;
    }
    getline(myfile, line);
    size_t pos = line.find(" ");
    Light.x = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    Light.y = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    Light.z = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    Light.r = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    Light.g = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    Light.b = stof(line);
    getline(myfile, line);
    pos = line.find(" ");
    Viewer[0] = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    Viewer[1] = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    Viewer[2] = stof(line);
    getline(myfile, line);
    pos = line.find(" ");
    IA[0] = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(" ");
    IA[1] = stof(line.substr(0, pos));
    line.erase(0, pos + 1);
    IA[2] = stof(line);
    myfile.close();
  }
}

void save()
{
  // Save the data to 3dcoordinates.txt , runs before quitting
  int n;
  Coord *C;
  Tri *L;
  ofstream myfile(filestring);
  if (myfile.is_open())
  {
    myfile << numshape << "\n";
    int i = 0;
    while (i < numshape) {
      n = numTri[i][0];
      myfile << n << "\n";
      int j = 0;
      while (j < n) {
        C = Coordinates[i][j];
        myfile << C->x << " " << C->y << " " << C->z << " " << C->r << " " << C->g << " " << C->b << "\n";
        j++;
      }
      n = numTri[i][1];
      myfile << n << "\n";
      j = 0;
      while (j < n) {
        L = Lines[i][j];
        myfile << (L->num1) << " " << (L->num2) << " " << (L->num3) << "\n";
        j++;
      }
      i++;
    }
    myfile << Light.x << " " << Light.y << " " << Light.z << " " << Light.r << " " << Light.b << " " << Light.b << "\n";
    myfile << Viewer[0] << " " << Viewer[1] << " " << Viewer[2] << "\n";
    myfile << IA[0] << " " << IA[1] << " " << IA[2] << "\n";
    myfile.close();
  }
}



void menu(int num)
{
  if (num == 0)
  {
    save();
    glutDestroyWindow(MainWindow);
    exit(0);
  }
  else
  {
    menuValue = num;
  }
  glutPostRedisplay();
}

void createmenu()
{
  int S1 = glutCreateMenu(menu);
  glutAddMenuEntry("Rotate", 80);
  glutAddMenuEntry("Translate", 90);
  int S2 = glutCreateMenu(menu);
  glutAddMenuEntry("Rotate", 81);
  glutAddMenuEntry("Translate", 91);
  int S3 = glutCreateMenu(menu); 
  glutAddMenuEntry("Rotate", 82);
  glutAddMenuEntry("Translate", 92);
  int S4 = glutCreateMenu(menu);
  glutAddMenuEntry("Rotate", 83);
  glutAddMenuEntry("Translate", 93);
  int S5 = glutCreateMenu(menu);
  glutAddMenuEntry("Rotate", 84);
  glutAddMenuEntry("Translate", 94); 
  Move_id = glutCreateMenu(menu);
  glutAddSubMenu("Shape 1", S1);
  if (numshape == 2) 
    glutAddSubMenu("Shape 2", S2);
  else if (numshape == 3)
    glutAddSubMenu("Shape 3", S3);
  else if (numshape == 4)
    glutAddSubMenu("Shape 4", S4);
  else if (numshape == 5)
    glutAddSubMenu("Shape 5", S5);
  View_id = glutCreateMenu(menu);
  glutAddMenuEntry("New Viewer Coordinates (input)", 11);
  glutAddMenuEntry("Move Viewer (Arrow Keys)", 12);
  Light_id = glutCreateMenu(menu);
  glutAddMenuEntry("New Light Coordinates (input)", 21);
  glutAddMenuEntry("Move Light (Arrow Keys)", 22);
  glutAddMenuEntry("New Light Values (input)", 23);
  glutAddMenuEntry("Adjust Light Values (Arrow Keys)", 24);
  glutAddMenuEntry("New Ambient Light Values (input)", 25);
  glutAddMenuEntry("Adjust Ambient Light Values (Arrow Keys)", 26);
  int men1 = glutCreateMenu(menu);
  glutAddMenuEntry("Ka", 31);
  glutAddMenuEntry("Kd", 32);
  glutAddMenuEntry("Ks", 33);
  glutAddMenuEntry("n (reflection)", 34);
  int men2 = glutCreateMenu(menu);
  glutAddMenuEntry("Ka", 41);
  glutAddMenuEntry("Kd", 42);
  glutAddMenuEntry("Ks", 43);
  glutAddMenuEntry("n (reflection)", 44);
  int men3 = glutCreateMenu(menu);
  glutAddMenuEntry("Ka", 51);
  glutAddMenuEntry("Kd", 52);
  glutAddMenuEntry("Ks", 53);
  glutAddMenuEntry("n (reflection)", 54);
  int men4 = glutCreateMenu(menu);
  glutAddMenuEntry("Ka", 61);
  glutAddMenuEntry("Kd", 62);
  glutAddMenuEntry("Ks", 63);
  glutAddMenuEntry("n (reflection)", 64);
  int men5 = glutCreateMenu(menu);
  glutAddMenuEntry("Ka", 71);
  glutAddMenuEntry("Kd", 72);
  glutAddMenuEntry("Ks", 73);
  glutAddMenuEntry("n (reflection)", 74);
  Phong_id = glutCreateMenu(menu);
  glutAddSubMenu("Shape 1", men1);
  if (numshape == 2)
    glutAddSubMenu("Shape 2", men2);
  else if (numshape == 3)
    glutAddSubMenu("Shape 3", men3);
  else if (numshape == 4)
    glutAddSubMenu("Shape 4", men4);
  else if (numshape == 5)
    glutAddSubMenu("Shape 5", men5);
  Menu_id = glutCreateMenu(menu);
  glutAddSubMenu("Move Shape", Move_id);
  glutAddSubMenu("Phong", Phong_id);
  glutAddSubMenu("Light", Light_id);
  glutAddSubMenu("Viewer", View_id);
  glutAddMenuEntry("Coordinate Color", 1);
  glutAddMenuEntry("Halftone", 2);
  glutAddMenuEntry("Exit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
 


void drawline(float *Buffer, int x, int y, bool flat)
{ // not only for xy plotting, just easiest syntax
  if(flat == false)
  {
    int P;
    int dx, dy, x1 = (x % 900) / 3, x2 = (y % 900) / 3, y1 = x / 900, y2 = y / 900;
    
    if (abs(y2 - y1) > abs(x2 - x1))
    //else
    {
      if (y2 < y1)
      {

        x1 = x1 + x2;
        x2 = x1 - x2;
        x1 = x1 - x2;

        y1 = y1 + y2;
        y2 = y1 - y2;
        y1 = y1 - y2;

        x = x + y;  
        y = x - y;   
        x = x - y;  

      }

      float r = Buffer[x], g = Buffer[x + 1], b = Buffer[x + 2];
      dy = y2 - y1;
      float dr = (Buffer[y] - r) / dy, dg = (Buffer[y + 1] - g) / dy, db = (Buffer[y + 2] - b) / dy;
    
      if (x1 < x2)
      {
        dx = x2 - x1;
        P = 2 * dx - dy;
        while (x < y)
        {
          Buffer[x] = r;
          Buffer[x + 1] = g;
          Buffer[x + 2] = b;
          if (lineloc[y1][0] == -1)
            lineloc[y1][0] = lineloc[y1][1] = x;
          else if (lineloc[y1][0] > x)
            lineloc[y1][0] = x;
          else if (lineloc[y1][1] < x)
            lineloc[y1][1] = x;
          x += 900;
          y1++;
          r += dr;
          g += dg;
          b += db;
          if (P < 0)
          {
            P = P + 2 * dx;
          }
          else {
            x += 3;
            P = P + 2 * dx - 2 * dy;
          }
        }
      }
      else
      {
        dx = x1 - x2;
        P = 2 * dx - dy;
        while (x < y)
        {
          Buffer[x] = r;
          Buffer[x + 1] = g;
          Buffer[x + 2] = b;
          if (lineloc[y1][0] == -1)
            lineloc[y1][0] = lineloc[y1][1] = x;
          else if (lineloc[y1][0] > x)
            lineloc[y1][0] = x;
          else if (lineloc[y1][1] < x)
            lineloc[y1][1] = x;
          x += 900;
          y1++;
          r += dr;
          g += dg;
          b += db;
          
          if (P <= 0) {
            P = P + 2 * dx;
          }
          else {
            x -= 3;
            P = P + 2 * dx - 2 * dy;
          }

        }
      }
      

    }


    else
    {
      if (x2 < x1)
      {
       
        x1 = x1 + x2;
        x2 = x1 - x2;
        x1 = x1 - x2;

        y1 = y1 + y2;
        y2 = y1 - y2;
        y1 = y1 - y2;

        x = x + y;  
        y = x - y;   
        x = x - y;  
        
      }
      float r = Buffer[x], g = Buffer[x + 1], b = Buffer[x + 2];
      dx = x2 - x1;
      float dr = (Buffer[y] - r) / dx, dg = (Buffer[y + 1] - g) / dx, db = (Buffer[y + 2] - b) / dx;

      
      if (y2 < y1)
      
      {
        dy = y1 - y2;
        P = 2 * dy - dx;
        while ((x % 900) < (y % 900))
        {
          Buffer[x] = r;
          Buffer[x + 1] = g;
          Buffer[x + 2] = b;
          if (lineloc[y1][0] == -1)
            lineloc[y1][0] = lineloc[y1][1] = x;
          else if (lineloc[y1][0] > x)
            lineloc[y1][0] = x;
          else if (lineloc[y1][1] < x)
            lineloc[y1][1] = x;
          x += 3;
          r += dr;
          g += dg;
          b += db;
          if (P > 0)
          {
            x -= 900;
            y1--;
            P = P + 2 * dy - 2 * dx;
          }
          else
            P = P + 2 * dy;
        }
      }

      else
      {
        dy = y2 - y1;
        P = 2 * dy - dx;
        while (x < y)
        {
          Buffer[x] = r;
          Buffer[x + 1] = g;
          Buffer[x + 2] = b;
          if (lineloc[y1][0] == -1)
            lineloc[y1][0] = lineloc[y1][1] = x;
          else if (lineloc[y1][0] > x)
            lineloc[y1][0] = x;
          else if (lineloc[y1][1] < x)
            lineloc[y1][1] = x;
          x += 3;
          r += dr;
          g += dg;
          b += db;
          if (P > 0)
          {
            x += 900;
            y1++;
            P = P + 2 * dy - 2 * dx;
          }
          else
            P = P + 2 * dy;
        }
      }
    }

  }

  else
  {
    float r = Buffer[x], g = Buffer[x + 1], b = Buffer[x + 2];
    int dx = (y - x) / 3;
    float dr = (Buffer[y] - r) / dx, dg = (Buffer[y + 1] - g) / dx, db = (Buffer[y + 2] - b) / dx;
    for (int i = 0; i < dx; i++)
    {
      x += 3;
      r += dr;
      Buffer[x] = r;
      g += dg;
      Buffer[x + 1] = g;
      b += db;
      Buffer[x + 2] = b;
    }
  }
  
}


void display1()
{
  glutSetWindow(xyWindow); 
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < 270000; i++)
  {
    PixelBuffer1[i] = 0;
  }
  int drawmin, drawmax, cor;
  PaintOrder.makeEmpty();
  Tri *L;
  float vertcol[9]; // vertcol[i][j] ==> The color of vertex i, j=0 ==> r, j=1 ==> g, j=2 ==> b
  
  int i = 0;
  while (i < numshape) {
    int j = 0;
    while (j < numTri[i][1]) {
      L = Lines[i][j];
      L->sort = 2;
      PaintOrder.insert(L);
      j++;
    }
    i++;
  }

  PaintOrder.sort();
  int y;
  while (!PaintOrder.isEmpty())
  {

    i = 0;
    while (i < 300) {
      lineloc[i][0] = lineloc[i][1] = -1;
      i++;
    }

    L = PaintOrder.deleteMax();
    drawmin = (findmin(L->C1->y, L->C2->y, L->C3->y, L->C3->y) - Minimum) * scalfac;
    drawmax = (findmax(L->C1->y, L->C2->y, L->C3->y, L->C3->y) - Minimum) * scalfac;

    Phong(vertcol, L, Light, IA, Viewer);
    cor = L->C1->corxy;
 
    i = 0;
    while (i < 3) {
      PixelBuffer1[cor + i] = vertcol[i];
      i++;
    }

    y = cor / 900;
    lineloc[y][0] = lineloc[y][1] = cor;
    cor = L->C2->corxy;


    i = 0;
    while (i < 3) {
      PixelBuffer1[cor + i] = vertcol[i+3];
      i++;
    }

    y = cor / 900;
    if (lineloc[y][0] == -1)
      lineloc[y][0] = lineloc[y][1] = cor;
    else if (lineloc[y][0] > cor)
      lineloc[y][0] = cor;
    else if (lineloc[y][1] < cor)
      lineloc[y][1] = cor;
    cor = L->C3->corxy;
    for (int i = 0; i < 3; i++) {
      PixelBuffer1[cor + i] = vertcol[i+6];
    }
 
    y = cor / 900;
    if (lineloc[y][0] == -1)
      lineloc[y][0] = lineloc[y][1] = cor;
    else if (lineloc[y][0] > cor)
      lineloc[y][0] = cor;
    else if (lineloc[y][1] < cor)
      lineloc[y][1] = cor;

    drawline(PixelBuffer1, L->C1->corxy, L->C2->corxy, false);
    drawline(PixelBuffer1, L->C2->corxy, L->C3->corxy, false);
    drawline(PixelBuffer1, L->C1->corxy, L->C3->corxy, false);

    i = drawmin;
    while (i <= drawmax) {
      drawline(PixelBuffer1, lineloc[i][0], lineloc[i][1], true);
      i++; 
    }

  }
  y = (int(((Light.y) - Minimum) * scalfac) * 300 + int(((Light.x) - Minimum) * scalfac)) * 3;
  if ((y % 900 > 0) && (y % 900 < 897) && (y / 900 > 0) && (y / 900 < 299))
  {
    for (int i = 0; i < 3; i++) {
      if (i == 0) { PixelBuffer1[y + i] = PixelBuffer1[y - 903 - i] = PixelBuffer1[y - 897 - i] = PixelBuffer1[y + 897 + i] = PixelBuffer1[y + 903 + i] = Light.r; }
      else if (i == 1) { PixelBuffer1[y + i] = PixelBuffer1[y - 903 - i] = PixelBuffer1[y - 897 - i] = PixelBuffer1[y + 897 + i] = PixelBuffer1[y + 903 + i] = Light.g; }
      else if (i == 2) { PixelBuffer1[y + i] = PixelBuffer1[y - 903 - i] = PixelBuffer1[y - 897 - i] = PixelBuffer1[y + 897 + i] = PixelBuffer1[y + 903 + i] = Light.b;}
    }
  }
  y = (int((Viewer[1] - Minimum) * scalfac) * 300 + int((Viewer[0] - Minimum) * scalfac)) * 3;
  if ((y % 900 > 0) && (y % 900 < 897) && (y / 900 > 0) && (y / 900 < 299))
  {
    PixelBuffer1[y - 3] = PixelBuffer1[y - 903] = PixelBuffer1[y - 897] = PixelBuffer1[y + 897] = PixelBuffer1[y + 903] = 1;
    PixelBuffer1[y + 3] = PixelBuffer1[y + 900] = PixelBuffer1[y - 900] = 1;
    PixelBuffer1[y - 2] = PixelBuffer1[y - 902] = PixelBuffer1[y - 896] = PixelBuffer1[y + 898] = PixelBuffer1[y + 904] = 1;
    PixelBuffer1[y + 4] = PixelBuffer1[y + 901] = PixelBuffer1[y - 899] = 1;
    PixelBuffer1[y + 2] = PixelBuffer1[y - 901] = PixelBuffer1[y - 895] = PixelBuffer1[y + 899] = PixelBuffer1[y + 905] = 1;
    PixelBuffer1[y + 5] = PixelBuffer1[y + 902] = PixelBuffer1[y - 898] = 1;
  }
  if (halftoneCheck)
    Halftone(PixelBuffer1);

  i=0;
  while (i < 900) {
    PixelBuffer1[i] = 1;
    i+=3;
  }
  i=0;
  while (i < 270000) {
    PixelBuffer1[i] = 1;
    i+=900;
  }
  i=897;
  while (i < 270000) {
    PixelBuffer1[i] = 1;
    i+=900;
  }
  i=269100;
  while (i < 270000) {
    PixelBuffer1[i] = 1;
    i+=3;
  }

  glLoadIdentity();
  glDrawPixels(300, 300, GL_RGB, GL_FLOAT, PixelBuffer1);
  glEnd();
  glFlush();
}

void display2()
{
  glutSetWindow(xzWindow);
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (int i = 0; i < 270000; i++)
  {
    PixelBuffer2[i] = 0;
  }

  int drawmin, drawmax, cor;
  PaintOrder.makeEmpty();
  Tri *L;
  float vertcol[9]; // vertcol[i][j] ==> The color of vertex i, j=0 ==> r, j=1 ==> g, j=2 ==> b
  
  int i = 0;
  while (i < numshape) {
    int j = 0;
    while (j < numTri[i][1]) {
      L = Lines[i][j];
      L->sort = 1;
      PaintOrder.insert(L);
      j++;
    }
    i++;
  }


  PaintOrder.sort();
  int z;
  while (!PaintOrder.isEmpty())
  {

    i = 0;
    while (i < 300) {
      lineloc[i][0] = lineloc[i][1] = -1;
      i++;
    }

    L = PaintOrder.deleteMax();
    drawmin = (findmin(L->C1->z, L->C2->z, L->C3->z, L->C3->z) - Minimum) * scalfac;
    drawmax = (findmax(L->C1->z, L->C2->z, L->C3->z, L->C3->z) - Minimum) * scalfac;

    Phong(vertcol, L, Light, IA, Viewer);
    cor = L->C1->corxz;

    i = 0;
    while (i < 3) {
      PixelBuffer2[cor + i] = vertcol[i];
      i++;
    }

    z = cor / 900;
    lineloc[z][0] = lineloc[z][1] = cor;
    cor = L->C2->corxz;
 
    i = 0;
    while (i < 3) {
      PixelBuffer2[cor + i] = vertcol[i+3];
      i++;
    }

    z = cor / 900;
    if (lineloc[z][0] == -1)
      lineloc[z][0] = lineloc[z][1] = cor;
    else if (lineloc[z][0] > cor)
      lineloc[z][0] = cor;
    else if (lineloc[z][1] < cor)
      lineloc[z][1] = cor;
    cor = L->C3->corxz;

    i = 0;
    while (i < 3) {
      PixelBuffer2[cor + i] = vertcol[i+6];
      i++;
    }        

    z = cor / 900;
    if (lineloc[z][0] == -1)
      lineloc[z][0] = lineloc[z][1] = cor;
    else if (lineloc[z][0] > cor)
      lineloc[z][0] = cor;
    else if (lineloc[z][1] < cor)
      lineloc[z][1] = cor;
    drawline(PixelBuffer2, L->C1->corxz, L->C2->corxz, false);
    drawline(PixelBuffer2, L->C2->corxz, L->C3->corxz, false);
    drawline(PixelBuffer2, L->C1->corxz, L->C3->corxz, false);

    i = drawmin;
    while (i <= drawmax) {
      drawline(PixelBuffer2, lineloc[i][0], lineloc[i][1], true);
      i++;
    }
  }
  z = (int(((Light.z) - Minimum) * scalfac) * 300 + int(((Light.x) - Minimum) * scalfac)) * 3;
  if ((z % 900 > 0) && (z % 900 < 897) && (z / 900 > 0) && (z / 900 < 299))
  {
    for (int i = 0; i < 3; i++) {
      if (i == 0) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.r; }
      else if (i == 1) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.g; }
      else if (i == 2) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.b;}
    }

  }
  z = (int((Viewer[2] - Minimum) * scalfac) * 300 + int((Viewer[0] - Minimum) * scalfac)) * 3;
  if ((z % 900 > 0) && (z % 900 < 897) && (z / 900 > 0) && (z / 900 < 299))
  {
    PixelBuffer2[z - 3] = PixelBuffer2[z - 903] = PixelBuffer2[z - 897] = PixelBuffer2[z + 897] = PixelBuffer2[z + 903] = 1;
    PixelBuffer2[z + 3] = PixelBuffer2[z + 900] = PixelBuffer2[z - 900] = 1;
    PixelBuffer2[z - 2] = PixelBuffer2[z - 902] = PixelBuffer2[z - 896] = PixelBuffer2[z + 898] = PixelBuffer2[z + 904] = 1;
    PixelBuffer2[z + 4] = PixelBuffer2[z + 901] = PixelBuffer2[z - 899] = 1;
    PixelBuffer2[z + 2] = PixelBuffer2[z - 901] = PixelBuffer2[z - 895] = PixelBuffer2[z + 899] = PixelBuffer2[z + 905] = 1;
    PixelBuffer2[z + 5] = PixelBuffer2[z + 902] = PixelBuffer2[z - 898] = 1;
  }
  if (halftoneCheck)
    Halftone(PixelBuffer2);

  i = 0;
  while (i < 900) {
    PixelBuffer2[i] = 1;
    i+=3;
  }
  i = 0;
  while (i < 270000) {
    PixelBuffer2[i] = 1;
    i+=900;
  } 
  i = 898;
  while (i < 270000) {
    PixelBuffer2[i] = 1;
    i+=900;
  }
  i = 269101;
  while (i < 270000) {
    PixelBuffer2[i] = 1;
    i+=3;
  }
  glLoadIdentity();
  glDrawPixels(300, 300, GL_RGB, GL_FLOAT, PixelBuffer2);
  glEnd();
  glFlush();
}

void display3()
{
  
  glutSetWindow(yzWindow);
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < 270000; i++)
  {
    PixelBuffer3[i] = 0; 
  }
  int drawmin, drawmax, cor;
  PaintOrder.makeEmpty();
  Tri *L;
  float vertcol[9]; // vertcol[i][j] ==> The color of vertex i, j=0 ==> r, j=1 ==> g, j=2 ==> b
   
  int i = 0;
  while (i < numshape) {
    int j = 0;
    while (j < numTri[i][1]) {
      L = Lines[i][j];
      L->sort = 0;
      PaintOrder.insert(L);
      j++;
    }
    i++;
  }

  int z;
  PaintOrder.sort();
  while (!PaintOrder.isEmpty())
  {

    i = 0;
    while (i < 300) {
      lineloc[i][0] = lineloc[i][1] = -1;
      i++;
    }
    L = PaintOrder.deleteMax();
    drawmin = (findmin(L->C1->z, L->C2->z, L->C3->z, L->C3->z) - Minimum) * scalfac;
    drawmax = (findmax(L->C1->z, L->C2->z, L->C3->z, L->C3->z) - Minimum) * scalfac;
    Phong(vertcol, L, Light, IA, Viewer);
    cor = L->C1->coryz;

    i = 0;
    while (i < 3) {
      PixelBuffer3[cor + i] = vertcol[i];
      i++;
    }

    z = cor / 900;
    lineloc[z][0] = lineloc[z][1] = cor;
    cor = L->C2->coryz;


    i = 0;
    while (i < 3) {
      PixelBuffer3[cor + i] = vertcol[i+3];
      i++;
    }

    z = cor / 900;
    if (lineloc[z][0] == -1)
      lineloc[z][0] = lineloc[z][1] = cor;
    else if (lineloc[z][0] > cor)
      lineloc[z][0] = cor;
    else if (lineloc[z][1] < cor)
      lineloc[z][1] = cor;
    cor = L->C3->coryz;
    for (int i = 0; i < 3; i++) {
      PixelBuffer3[cor + i] = vertcol[i+6];
    }

    z = cor / 900;
    if (lineloc[z][0] == -1)
      lineloc[z][0] = lineloc[z][1] = cor;
    else if (lineloc[z][0] > cor)
      lineloc[z][0] = cor;
    else if (lineloc[z][1] < cor)
      lineloc[z][1] = cor;
    drawline(PixelBuffer3, L->C1->coryz, L->C2->coryz, false);
    drawline(PixelBuffer3, L->C2->coryz, L->C3->coryz, false);
    drawline(PixelBuffer3, L->C1->coryz, L->C3->coryz, false);

    i = drawmin;
    while (i <= drawmax) {
      drawline(PixelBuffer3, lineloc[i][0], lineloc[i][1], true);
      i++;
    }
  }
  z = (int(((Light.z) - Minimum) * scalfac) * 300 + int(((Light.y) - Minimum) * scalfac)) * 3;
  if ((z % 900 > 0) && (z % 900 < 897) && (z / 900 > 0) && (z / 900 < 299))
  {
    for (int i = 0; i < 3; i++) {
      if (i == 0) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.r; }
      else if (i == 1) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.g; }
      else if (i == 2) { PixelBuffer1[z + i] = PixelBuffer1[z - 903 - i] = PixelBuffer1[z - 897 - i] = PixelBuffer1[z + 897 + i] = PixelBuffer1[z + 903 + i] = Light.b;}
    }

  }
  z = (int((Viewer[2] - Minimum) * scalfac) * 300 + int((Viewer[1] - Minimum) * scalfac)) * 3;
  if ((z % 900 > 0) && (z % 900 < 897) && (z / 900 > 0) && (z / 900 < 299))
  {
    PixelBuffer3[z - 3] = PixelBuffer3[z - 903] = PixelBuffer3[z - 897] = PixelBuffer3[z + 897] = PixelBuffer3[z + 903] = 1;
    PixelBuffer3[z + 3] = PixelBuffer3[z + 900] = PixelBuffer3[z - 900] = 1;
    PixelBuffer3[z - 2] = PixelBuffer3[z - 902] = PixelBuffer3[z - 896] = PixelBuffer3[z + 898] = PixelBuffer3[z + 904] = 1;
    PixelBuffer3[z + 4] = PixelBuffer3[z + 901] = PixelBuffer3[z - 899] = 1;
    PixelBuffer3[z + 2] = PixelBuffer3[z - 901] = PixelBuffer3[z - 895] = PixelBuffer3[z + 899] = PixelBuffer3[z + 905] = 1;
    PixelBuffer3[z + 5] = PixelBuffer3[z + 902] = PixelBuffer3[z - 898] = 1;
  }
  if (halftoneCheck)
    Halftone(PixelBuffer3);

  i = 2;
  while (i < 900) {
    PixelBuffer3[i] = 1;
    i+=3;
  }
  i = 2;
  while (i < 270000) {
    PixelBuffer3[i] = 1;
    i+=900;
  } 
  i = 899;
  while (i < 270000) {
    PixelBuffer3[i] = 1;
    i+=900; 
  }
  i = 269102;
  while (i < 270000) {
    PixelBuffer3[i] = 1;
    i+=3;
  }
  glLoadIdentity();
  glDrawPixels(300, 300, GL_RGB, GL_FLOAT, PixelBuffer3);
  glEnd();
  glFlush();
} 


void norm() 
{
  Coord *C;
  Tri *L; 
  float vec1[3], vec2[3];
  for (int i = 0; i < numshape; i++)
  {
    for (int j = 0; j < numTri[i][0]; j++)
    {
      C = Coordinates[i][j];
      C->r = (C->r > 1) ? 1 : C->r; 
      C->r = (C->r < 0) ? 0 : C->r; 
      C->g = (C->g > 1) ? 1 : C->g; 
      C->g = (C->g < 0) ? 0 : C->g; 
      C->b = (C->b > 1) ? 1 : C->b; 
      C->b = (C->b < 0) ? 0 : C->b; 

      vec1[0] = (((C->y) - Minimum) * scalfac);
      vec2[0] = ((C->x) - Minimum) * scalfac;
      vec1[1] = (((C->z) - Minimum) * scalfac);
      C->corxy = (int(vec1[0]) * 300 + int(vec2[0])) * 3;
      C->corxz = (int(vec1[1]) * 300 + int(vec2[0])) * 3; 
      C->coryz = (int(vec1[1]) * 300 + int(vec1[0])) * 3;
      C->corxy -= (C->corxy % 3);
      C->corxz -= (C->corxz % 3);
      C->coryz -= (C->coryz % 3);
    }
    for (int j = 0; j < numTri[i][1]; j++)
    {
      L = Lines[i][j];
      vec1[0] = L->C1->x - L->C2->x;
      vec1[1] = L->C1->y - L->C2->y;
      vec1[2] = L->C1->z - L->C2->z;
      vec2[0] = L->C1->x - L->C3->x;
      vec2[1] = L->C1->y - L->C3->y;
      vec2[2] = L->C1->z - L->C3->z;


      L->normvec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
      L->normvec[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
      L->normvec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
      Normalize(L->normvec);
      float lightvec[3] = {Light.x - L->C1->x, Light.y - L->C1->y, Light.z - L->C1->z};
      if (Dot(L->normvec, lightvec) < 0)
      {
        L->normvec[0] = 0 - L->normvec[0];
        L->normvec[1] = 0 - L->normvec[1];
        L->normvec[2] = 0 - L->normvec[2];
      }
      L->plane = -(L->normvec[0] * L->C1->x + L->normvec[1] * L->C1->y + L->normvec[2] * L->C1->z);

    }
  }

}


void draw() //main window isnt actually used
{
  glutSetWindow(MainWindow);
  glClear(GL_COLOR_BUFFER_BIT); //clears the colour and depth buffers
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2f(0.1, -.45);
  if (sign == -1)
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"LEFT (Negate)  ~X~");
  else
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"LEFT (Negate)");
  glRasterPos2f(0.4, -.1);
  if (DeltaZ != 0)
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"UP (move Z) ~X~");
  else
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"UP (move Z)");
  glRasterPos2f(0.5, -.55);
  if (DeltaY != 0)
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"RIGHT (move Y)  ~X~");
  else
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"RIGHT (move Y)");
  glRasterPos2f(0.4, -.9);
  if (DeltaX != 0)
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"DOWN (move X)  ~X~");
  else
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"DOWN (move X)");
  glEnd();
  glFlush();
}



void drawall()
{
  int i,j,K,n;
  switch (menuValue)
  {
  case 1:
    //int i, j;
    cout << "Shape Number (Int 1 - " << numshape << "): ";
    cin >> i;
    i--;
    if (i > numshape)
      i = numshape; 
    else if (i < 0)
      i = 0;
    cout << "Coordinate Number (Int 1 - " << numTri[i][0] << "): ";
    cin >> j;
    j--;
    if (j > numTri[i][0])
      j = numTri[i][0];
    else if (j < 0)
      j = 0;
    cout << "Coordinate R Value: ";
    cin >> Coordinates[i][j]->r;
    cout << "Coordinate G Value: ";
    cin >> Coordinates[i][j]->g;
    cout << "Coordinate B Value: ";
    cin >> Coordinates[i][j]->b;
    menuValue = 24;
    break;
  case 2:
    halftoneCheck = !halftoneCheck;
    menuValue = 24;
    break;
  case 11:
    cout << "Viewer X coordinate: ";
    cin >> Viewer[0];
    cout << "Viewer Y coordinate: ";
    cin >> Viewer[1];
    cout << "Viewer Z coordinate: ";
    cin >> Viewer[2];
    menuValue++;
    break;
  case 12:
    Viewer[0] += DeltaX / 100 * sign * (Maximum - Minimum);
    Viewer[1] += DeltaY / 100 * sign * (Maximum - Minimum);
    Viewer[2] += DeltaZ / 100 * sign * (Maximum - Minimum);
    break;
  case 21:
    cout << "Light X coordinate: ";
    cin >> Light.x;
    cout << "Light Y coordinate: ";
    cin >> Light.y;
    cout << "Light Z coordinate: ";
    cin >> Light.z;
    menuValue++;
    setLight(Light, IA);
    break;
  case 22:
    Light.x += DeltaX / 100 * sign * (Maximum - Minimum);
    Light.y += DeltaY / 100 * sign * (Maximum - Minimum);
    Light.z += DeltaZ / 100 * sign * (Maximum - Minimum);
    //setLight();
    setLight(Light, IA);
    break;
  case 23:
    cout << "Light R value: ";
    cin >> Light.r;
    cout << "Light G value: ";
    cin >> Light.g;
    cout << "Light B value: ";
    cin >> Light.b;
    menuValue++;
    //setLight();
    setLight(Light, IA);
    break;
  case 24:
    Light.r += DeltaX / 100 * sign;
    Light.g += DeltaY / 100 * sign;
    Light.b += DeltaZ / 100 * sign;

    setLight(Light, IA);
    break;
  case 25:
    cout << "Ambient Light R value: ";
    cin >> IA[0];
    cout << "Ambient Light G value: ";
    cin >> IA[1];
    cout << "Ambient Light B value: ";
    cin >> IA[2];
    menuValue++;

    setLight(Light, IA);
    break;
  case 26:
    IA[0] += DeltaX / 100 * sign;
    IA[1] += DeltaY / 100 * sign;
    IA[2] += DeltaZ / 100 * sign;

    setLight(Light, IA);
    break;
  case 31:

    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ka Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KA = K;
    }
    menuValue = 24;

    break;
  case 32:
  
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Kd Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; i = j++)
    {
      Lines[i][j]->KD = K;
    }
    menuValue = 24;

    break;
  case 33:
   
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ks Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KS = K;
    }
    menuValue = 24;

    break;
  case 34:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " n Value (1, 2, 3..): ";
    cin >> n;
    n = (n < 1) ? 1 : n;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->n = n;
    }
    menuValue = 24;

    break;
  case 41:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ka Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KA = K;
    }
    menuValue = 24;

    break;
  case 42: 
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Kd Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; i = j++)
    {
      Lines[i][j]->KD = K;
    }
    menuValue = 24;

    break; 
  case 43: 
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ks Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KS = K;
    }
    menuValue = 24;
    break; 
  case 44: 
   
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " n Value (1, 2, 3..): ";
    cin >> n;
    n = (n < 1) ? 1 : n;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->n = n;
    }
    menuValue = 24;
    break;
  case 51:
   
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ka Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KA = K;
    }
    menuValue = 24;
    break;
  case 52:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Kd Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; i = j++)
    {
      Lines[i][j]->KD = K;
    }
    menuValue = 24;

    break;
  case 53:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ks Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KS = K;
    }
    menuValue = 24;
    break;
  case 54:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " n Value (1, 2, 3..): ";
    cin >> n;
    n = (n < 1) ? 1 : n;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->n = n;
    }
    menuValue = 24;
    break;
  case 61:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ka Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KA = K;
    }
    menuValue = 24;
    break;
  case 62:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Kd Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; i = j++)
    {
      Lines[i][j]->KD = K;
    }
    menuValue = 24;

    break;
  case 63:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ks Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KS = K;
    }
    menuValue = 24;
    break;
  case 64:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " n Value (1, 2, 3..): ";
    cin >> n;
    n = (n < 1) ? 1 : n;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->n = n;
    }
    menuValue = 24;
    break;
  case 71:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ka Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KA = K;
    }
    menuValue = 24;
    break;
  case 72:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Kd Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; i = j++)
    {
      Lines[i][j]->KD = K;
    }
    menuValue = 24;

    break;
  case 73:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " Ks Value (0 - 1): ";
    cin >> K;
    K = (K > 1) ? 1 : K;
    K = (K < 0) ? 0 : K;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->KS = K;
    }
    menuValue = 24;
    break;
  case 74:
    
    i = (menuValue / 10) - 3;
    cout << "Shape " << i + 1 << " n Value (1, 2, 3..): ";
    cin >> n;
    n = (n < 1) ? 1 : n;
    for (j = 0; j < numTri[i][1]; j++)
    {
      Lines[i][j]->n = n;
    }
    menuValue = 24;
    break;
  case 80:
    Rotate(menuValue % 10, DeltaX, DeltaY, DeltaZ, numTri, Coordinates, sign);
    break;
  case 81:
    Rotate(menuValue % 10, DeltaX, DeltaY, DeltaZ, numTri, Coordinates, sign);
    break;
  case 82:
    Rotate(menuValue % 10, DeltaX, DeltaY, DeltaZ, numTri, Coordinates, sign);
    break;
  case 83:
    Rotate(menuValue % 10, DeltaX, DeltaY, DeltaZ, numTri, Coordinates, sign);
    break; 
  case 84:
    Rotate(menuValue % 10, DeltaX, DeltaY, DeltaZ, numTri, Coordinates, sign);
    break;
  case 90:
    Translate(menuValue % 10, DeltaX, DeltaY, DeltaZ, Maximum, Minimum, sign, Coordinates, numTri);
    break;
  case 91:
    Translate(menuValue % 10, DeltaX, DeltaY, DeltaZ, Maximum, Minimum, sign, Coordinates, numTri);
    break;
  case 92:
    Translate(menuValue % 10, DeltaX, DeltaY, DeltaZ, Maximum, Minimum, sign, Coordinates, numTri);
    break;
  case 93:
    Translate(menuValue % 10, DeltaX, DeltaY, DeltaZ, Maximum, Minimum, sign, Coordinates, numTri);
    break;
  case 94:
    Translate(menuValue % 10, DeltaX, DeltaY, DeltaZ, Maximum, Minimum, sign, Coordinates, numTri);
    break;
  }
  
  Coord *C;
  Maximum = Minimum = Viewer[0];
  Maximum = (Viewer[1] > Maximum) ? Viewer[1] : Maximum;
  Maximum = (Viewer[2] > Maximum) ? Viewer[2] : Maximum;
  Maximum = (Light.x > Maximum) ? Light.x : Maximum;
  Maximum = (Light.y > Maximum) ? Light.y : Maximum;
  Maximum = (Light.z > Maximum) ? Light.z : Maximum;
  Minimum = (Viewer[1] < Minimum) ? Viewer[1] : Minimum;
  Minimum = (Viewer[2] < Minimum) ? Viewer[2] : Minimum;
  Minimum = (Light.x < Minimum) ? Light.x : Minimum;
  Minimum = (Light.y < Minimum) ? Light.y : Minimum;
  Minimum = (Light.z < Minimum) ? Light.z : Minimum;
  i=0;
  while (i < numshape) {
    int j = 0;
    while (j < numTri[i][0]) {
      C = Coordinates[i][j];
      Maximum = findmax(C->x, C->y, C->z, Maximum);
      Minimum = findmin(C->x, C->y, C->z, Minimum);
      j++;  
    }
    i++;
  }

  Maximum++;
  Minimum--;
  scalfac = 299 / (Maximum - Minimum);


  Tri *L;
  float vec1[3], vec2[3];
  for (int i = 0; i < numshape; i++)
  {
    for (int j = 0; j < numTri[i][0]; j++)
    {
      C = Coordinates[i][j];
      C->r = (C->r > 1) ? 1 : C->r; 
      C->r = (C->r < 0) ? 0 : C->r; 
      C->g = (C->g > 1) ? 1 : C->g; 
      C->g = (C->g < 0) ? 0 : C->g; 
      C->b = (C->b > 1) ? 1 : C->b; 
      C->b = (C->b < 0) ? 0 : C->b; 

      vec1[0] = (((C->y) - Minimum) * scalfac);
      vec2[0] = ((C->x) - Minimum) * scalfac;
      vec1[1] = (((C->z) - Minimum) * scalfac);
      C->corxy = (int(vec1[0]) * 300 + int(vec2[0])) * 3;
      C->corxz = (int(vec1[1]) * 300 + int(vec2[0])) * 3; 
      C->coryz = (int(vec1[1]) * 300 + int(vec1[0])) * 3;
      C->corxy -= (C->corxy % 3);
      C->corxz -= (C->corxz % 3);
      C->coryz -= (C->coryz % 3);
    }
    for (int j = 0; j < numTri[i][1]; j++)
    {
      L = Lines[i][j];
      vec1[0] = L->C1->x - L->C2->x;
      vec1[1] = L->C1->y - L->C2->y;
      vec1[2] = L->C1->z - L->C2->z;
      vec2[0] = L->C1->x - L->C3->x;
      vec2[1] = L->C1->y - L->C3->y;
      vec2[2] = L->C1->z - L->C3->z;
      
    

      L->normvec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
      L->normvec[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
      L->normvec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
      Normalize(L->normvec);
      float lightvec[3] = {Light.x - L->C1->x, Light.y - L->C1->y, Light.z - L->C1->z};
      if (Dot(L->normvec, lightvec) < 0)
      {
        L->normvec[0] = 0 - L->normvec[0];
        L->normvec[1] = 0 - L->normvec[1];
        L->normvec[2] = 0 - L->normvec[2];
      }
      L->plane = -(L->normvec[0] * L->C1->x + L->normvec[1] * L->C1->y + L->normvec[2] * L->C1->z);

    }
  }

  draw();
  display1();
  display2();
  display3();
  if ((DeltaZ != 0) || (DeltaY != 0) || (DeltaX != 0))
  {
    glutSetWindow(MainWindow);
    glutPostRedisplay();
  }
}





void pressKey(int key, int xx, int yy)
{
  
  if (key == GLUT_KEY_UP) {
    DeltaZ = 1.0;
  } 
  else if (key == GLUT_KEY_DOWN) { 
    DeltaX = 1.0;
  }
  else if (key == GLUT_KEY_RIGHT) {
    DeltaY = 1.0;
  }

  glutSetWindow(MainWindow); 
  glutPostRedisplay();
}

void releaseKey(int key, int xx, int yy)
{

  if (key == GLUT_KEY_UP) {
    DeltaZ = 0;
  }
  else if (key == GLUT_KEY_DOWN) {
    DeltaX = 0;
  }
  else if (key == GLUT_KEY_RIGHT) {
    DeltaY = 0;
  }
  else if (key == GLUT_KEY_LEFT) {
    sign *= -1;
  }

  glutSetWindow(MainWindow);
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  load();
  glutInit(&argc, argv); //initializes the GLUT framework
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  MainWindow = glutCreateWindow("Proj3");
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  createmenu();
  glutDisplayFunc(drawall);
  xyWindow = glutCreateSubWindow(MainWindow, 0, 300, 300, 300);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display1);

  xzWindow = glutCreateSubWindow(MainWindow, 0, 0, 300, 300);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display2);

  yzWindow = glutCreateSubWindow(MainWindow, 300, 0, 300, 300);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display3);
  glutMainLoop(); //the main loop of the GLUT framework
}
