
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream>
#include <cstring>
#include <cmath>
#include "Raytrace.h"

#define RADIAN 0.01745 // Degree to radian conversion

using namespace std;

int MainWindow, MenuNumber, ShpereNumber, MenuValue;
int resolution, numlight, numplane, numsphere, Recursions;
light *Lights[10];
sphere *Spheres[10];
plane *Planes[10];
float angle, From[3], At[3], Up[3], *PixelBuffer, ambient[3];
bool UP, DOWN, RIGHT, LEFT=false;


void readFile(){ // Read the Text File
    string line;
    ifstream myfile("raytrace.txt");
    size_t pos;
    float X[10];
    if (myfile.is_open()){
        getline(myfile,line);
        resolution = stoi(line);
        getline(myfile, line);
        angle = stof(line);
        getline(myfile,line);
        Recursions = stoi(line);
        
        getline(myfile,line);
        for(int i=0; i < 3; i++) {
            pos = line.find(" ");
            From[i] = stof(line.substr(0,pos)); //Plane definition and points
            line.erase(0, pos+1);
        }

        getline(myfile,line);
        for(int i=0; i < 3; i++) {
            pos = line.find(" ");
            At[i] = stof(line.substr(0,pos)); //Plane definition and points
            line.erase(0, pos+1);
        }

    
        getline(myfile,line);
        for(int i=0; i < 3; i++) {
            pos = line.find(" ");
            Up[i] = stof(line.substr(0,pos)); //Plane definition and points
            line.erase(0, pos+1);
        }

        getline(myfile,line);
        for(int i=0; i < 3; i++) {
            pos = line.find(" ");
            ambient[i] = stof(line.substr(0,pos)); //Plane definition and points
            line.erase(0, pos+1);
        }

        
        getline(myfile,line);
        numlight = stoi(line);
        for(int i=0; i<numlight; i++){
            getline(myfile,line); 
            for (int j = 0; j < 6; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Light location and value
                line.erase(0, pos+1);
            }
            
            Lights[i] = new light(X);
        }
        
        getline(myfile, line);
        numplane = stoi(line);
        for (int i=0; i<numplane; i++){
            getline(myfile,line);

            for(int j=0; j < 4; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Plane definition and points
                line.erase(0, pos+1);
            }
            
            Planes[i] = new plane(X);
            getline(myfile,line);
            for(int j=0; j < 9; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Plane definition and points
                line.erase(0, pos+1);
            }  
            
            Planes[i] -> AddPoints(X);
            
            getline(myfile,line);
            for(int j=0; j < 10; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Plane definition and points
                line.erase(0, pos+1);
            }
           
            Planes[i]->KAdd(X);
        }
        
        getline(myfile, line);
        numsphere = stoi(line);
        for (int i=0; i<numsphere; i++){
            getline(myfile,line);
            for(int j=0; j < 6; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Plane definition and points
                line.erase(0, pos+1);
            }
          
            Spheres[i] = new sphere(X);
            
            getline(myfile,line);
            for(int j=0; j < 10; j++) {
                pos = line.find(" ");
                X[j] = stof(line.substr(0,pos)); //Plane definition and points
                line.erase(0, pos+1);
            }
           
            Spheres[i]->KAdd(X);
        }
    }
    myfile.close();
}

void save(){    //Writes the file, same set up as readFile
    ofstream myfile ("raytrace.txt");
    if (myfile.is_open()){
        myfile << resolution << "\n";
        myfile << angle << "\n";
        myfile << Recursions << "\n";
        myfile << From[0] << " " << From[1] << " " << From[2] << "\n";
        myfile << At[0] << " " << At[1] << " " << At[2] << "\n";
        myfile << Up[0] << " " << Up[1] << " " << Up[2] << "\n";
        myfile << ambient[0] << " " << ambient[1] << " " << ambient[2] << "\n";
        myfile << numlight << "\n";

        int i=0;
        while (i < numlight) {
            myfile << Lights[i]->Coordinate[0] << " " << Lights[i]->Coordinate[1] << " " << Lights[i]->Coordinate[2] << " " << Lights[i]->Color[0] << " " << Lights[i]->Color[1] << " " << Lights[i]->Color[2] << "\n";
            i++;
        }
        myfile << numplane << "\n";

        i=0;
        while (i < numplane) {
            myfile << Planes[i]->ABC[0] <<" "<< Planes[i]->ABC[1] << " " << Planes[i]->ABC[2] << " " << Planes[i]->D<<"\n";
            myfile << Planes[i]->Point1[0] << " " << Planes[i]->Point1[1] << " " << Planes[i]->Point1[2] << " " << Planes[i]->Point2[0] << " " << Planes[i]->Point2[1] << " " << Planes[i]->Point2[2] << " " << Planes[i]->Point3[0] << " " << Planes[i]->Point3[1] << " " << Planes[i]->Point3[2] << "\n";
            myfile << Planes[i]->K[0] << " " << Planes[i]->K[1] << " " << Planes[i]->K[2] << " " << Planes[i]->K[3] << " " << Planes[i]->K[4] << " " << Planes[i]->K[5] << " " << Planes[i]->K[6] << " " << Planes[i]->K[7] << " " << Planes[i]->K[8] << " " << Planes[i]->K[9] << "\n";
            i++;
        }
        myfile << numsphere << "\n";

        i=0;
        while (i < numsphere) {
            myfile << Spheres[i]->Coordinate[0] << " " << Spheres[i]->Coordinate[1] << " " << Spheres[i]->Coordinate[2] << " " << Spheres[i]->Ellipse[0] << " " << Spheres[i]->Ellipse[1] << " " << Spheres[i]->Ellipse[2] << "\n";
            myfile << Spheres[i]->K[0] << " " << Spheres[i]->K[1] << " " << Spheres[i]->K[2] << " " << Spheres[i]->K[3] << " " << Spheres[i]->K[4] << " " << Spheres[i]->K[5] << " " << Spheres[i]->K[6] << " " << Spheres[i]->K[7] << " " << Spheres[i]->K[8] << " " << Spheres[i]->K[9] << "\n";
            i++;
        }
        
    }
    myfile.close();
}

void menu(int num){ //Called from the menu, sets value of MenuValue
    switch(num) {
        case -1:
            save();
            glutDestroyWindow(MainWindow);
            exit(0);
        case 1:
            cout << "PLEASE INPUT NEW FROM POINT: ";
            cin >> From[0] >> From[1] >> From[2];
        case 2:
            cout << "PLEASE INPUT NEW AT POINT: ";
            cin >> At[0] >> At[1] >> At[2];
        case 3:
            cout << "PLEASE INPUT NEW UP POINT: ";
            cin >> Up[0] >> Up[1] >> Up[2];
        case 4:
            cout << "PLEASE INPUT NEW RECURSION LEVEL: ";
            cin >> Recursions;
            if (Recursions<=0){
                Recursions = 1;
            }
        case 5:
            cout << "PLEASE INPUT NEW ANGLE: ";
            cin >> angle;
            if ((angle <=0)||(angle>=90))
                angle = 45;
        default:
            MenuValue=num;
    }
   
    glutPostRedisplay();
}

void createmenu(){
    
    ShpereNumber = glutCreateMenu(menu);
    
    switch (numsphere) {
        case 1:
            glutAddMenuEntry("sphere 1", 20);
            break;
        case 2:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            break;
        case 3:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            break;
        case 4:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            break;
        case 5:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            break;
        case 6:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            glutAddMenuEntry("sphere 6", 25);
            break;
        case 7:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            glutAddMenuEntry("sphere 6", 25);
            glutAddMenuEntry("sphere 7", 26);
            break;
        case 8:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            glutAddMenuEntry("sphere 6", 25);
            glutAddMenuEntry("sphere 7", 26);
            glutAddMenuEntry("sphere 8", 27);
            break;
        case 9: 
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            glutAddMenuEntry("sphere 6", 25);
            glutAddMenuEntry("sphere 7", 26);
            glutAddMenuEntry("sphere 8", 27);
            glutAddMenuEntry("sphere 9", 28);
            break;
        case 10:
            glutAddMenuEntry("sphere 1", 20);
            glutAddMenuEntry("sphere 2", 21);
            glutAddMenuEntry("sphere 3", 22);
            glutAddMenuEntry("sphere 4", 23);
            glutAddMenuEntry("sphere 5", 24);
            glutAddMenuEntry("sphere 6", 25);
            glutAddMenuEntry("sphere 7", 26);
            glutAddMenuEntry("sphere 8", 27);
            glutAddMenuEntry("sphere 9", 28);
            glutAddMenuEntry("sphere 10",29);
            break;
    } 
    
    MenuNumber = glutCreateMenu(menu);
    glutAddMenuEntry("New From Point", 1);
    glutAddMenuEntry("New At Point", 2);
    glutAddMenuEntry("New Up Vector", 3);
    glutAddMenuEntry("New Recursion Level",4);
    glutAddMenuEntry("New Angle", 5);
    glutAddSubMenu("Move Sphere", ShpereNumber);
    glutAddMenuEntry("Exit", -1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void findreflection(float Ray[3], float Normal[3], float Reflection[3]){
    float d = -(Ray[0]*Normal[0] + Ray[1]*Normal[1] + Ray[2]*Normal[2]);  //-dot(Ray, Normal);

    for (int i = 0; i < 3; i++) {
        Reflection[i] = Ray[i] + 2 * d * Normal[i];
    }
} 

bool hitcheck(float Source[3], float Ray[3], float newSource[3], float newNormal[3], float *t, float K[10]){
    *t=-1;
    float newt;
    for (int i=0; i<numplane; i++){
        newt = Planes[i]->NearHit(Source, Ray);
        if (((newt>0.03)&&(newt<*t)) || (*t<.03)){
            Planes[i]->getSource(Ray, K, newNormal);

            for (int j = 0; j < 3; j++) {
                newSource[j] = Source[j] + Ray[j]*newt;
            }
            *t=newt;
        }
    }
    for (int i=0; i<numsphere; i++){
        newt = Spheres[i]->NearHit(Source, Ray);
        if (((newt>0.03)&&(newt<*t)) || (*t<.03)){

            for(int j = 0; j < 3; j++) {
                newSource[j] = Source[j] + Ray[j]*newt;
            }

            Spheres[i]->getSource(Ray, K, newSource, newNormal);
            *t=newt;
        }

    }
    if (*t>0.03) return true;
    else return false;
}

void Phong(float Surface[3], float ViewerVec[3], float Normal[3], float distance, float RGB[3], float K[10]){
    float reflection[3], lightvec[3], dist;
    light *curlight;
    bool shadow;
    findreflection(ViewerVec, Normal, reflection);

    for (int i = 0; i < 3; i++) {
        RGB[i] = ambient[i] * K[i];
    }

    for (int i=0; i<numlight; i++){ //For every light source
        curlight = Lights[i];

        for (int j = 0; j < 3; j++) {
            lightvec[j] = curlight->Coordinate[j]-Surface[j];
        }

        float newSource[3], newNormal[3], t, X[10];
        dist = normalize(lightvec);
        shadow = false;
        if (hitcheck(Surface, lightvec, newSource, newNormal, &t, X)){
            if (t < dist && (dot(lightvec,Normal)<0)){
                shadow=true;
            }
        }
        for (int j=0; j<3; j++){
            if (!shadow)
                RGB[j] += (K[3+j]*dot(Normal, lightvec)*curlight->Color[0]+ K[6+j]*pow(dot(reflection,lightvec),K[9])*curlight->Color[0]) / (distance + dist);
        }
    }
}

void RayTrace(float Source[3], float Ray[3], float RGB[3], int depth){
    if (depth == 0){

        for (int i = 0; i < 3; i++) {
            RGB[i] = 0;
        }
        return ;
    }
    float t, newSource[3], newNormal[3], rgb[3], K[9];
    if (hitcheck(Source, Ray, newSource, newNormal, &t, K)){
        normalize(newNormal);
        Phong(newSource, Ray, newNormal, t, RGB, K);
        float reflection[3];
        findreflection(Ray, newNormal, reflection);
        RayTrace(newSource, reflection, rgb, depth-1);
 
        for (int i = 0; i < 3; i++) {
            RGB[i] += rgb[i]*.08;
        }
        RayTrace(newSource, Ray, rgb, depth-1);
        for (int i = 0; i < 3; i++) {
            RGB[i] += rgb[i]*.08;
        }
        
        return ;
    }
    else{
        for (int i = 0; i < 3; i++) {
            RGB[i] = 0; 
        }
        return ;
    } 
}

void movesphere(int i){
    if (LEFT){
        if (DOWN)
            Spheres[i]->Coordinate[0]-=0.15;
        if (RIGHT)
            Spheres[i]->Coordinate[1]-=0.15;
        if (UP)
            Spheres[i]->Coordinate[2]-=0.15;
    }
    else{
        if (DOWN)
            Spheres[i]->Coordinate[0]+=0.15;
        if (RIGHT)
            Spheres[i]->Coordinate[1]+=0.15;
        if (UP)
            Spheres[i]->Coordinate[2]+=0.15;
    }
}

void move(){
    if (MenuValue >19 && MenuValue <30){
        movesphere(MenuValue-20);
        glutPostRedisplay();
    }
}

void display(){
    move();
    float BaseRay[3], Right[3], Ray[3], Up2[3], RGB[3], As[3], upangle, rightangle;
    float csu, csr, snu, snr; //cosine of upangle, cosine of rightangle, sin of upangle, sin of rightangle
    
    for(int i = 0; i < 3; i++) {
        BaseRay[i] = At[i]-From[i];
    }
    normalize(BaseRay);
    cross(Up, BaseRay, Right);
    normalize(Right);//Make the up vector perpendicular to ray vector
    cross(BaseRay, Right, Up2);
    int i = 0;
    while(i < resolution) {

        upangle = (float(i)/float(resolution)*2 - 1) * angle;
        int j = 0;
        while (j < resolution) {

            rightangle = (float(j)/float(resolution)*2 - 1) * angle;
            csr = cos(rightangle*RADIAN);
            csu = cos(upangle*RADIAN);
            snr = sin(rightangle*RADIAN);
            snu = sin(upangle*RADIAN);
            As[0] = csr * csu;
            As[1] = snr * csu;
            As[2] = snu * csr;

            for (int k = 0; k < 3; k++) {
                Ray[k] = BaseRay[k] * As[0];
                Ray[k] += Right[k] * As[1];  
                Ray[k] += Up2[k] * As[2];  
            }
            normalize(Ray);
            RGB[0] = RGB[1] = RGB[2] = 0;
            RayTrace(From, Ray, RGB, Recursions); //Recursive call, light values returned in RGB
            for(int k = 0; k < 3; k++) {
                PixelBuffer[j*3 + i*resolution*3+k] = RGB[k];    
            }
           j++;
        }
        i++;
    }
    csu = 0;
    for (int i=0; i<resolution * resolution * 3; i++){
        csu = (csu<PixelBuffer[i]) ? PixelBuffer[i] : csu;
    }
    if (!(csu==0)) {
        for (int i=0; i<resolution * resolution * 3; i++){
            PixelBuffer[i] /= csu;
        }
    }
    glDrawPixels(resolution, resolution, GL_RGB, GL_FLOAT, PixelBuffer);
    glFlush();
}

void pressKey(int key, int xx, int yy) { //Decide if arrow keys are pressed
    if (key == GLUT_KEY_UP) {UP = true;}
    else if (key == GLUT_KEY_DOWN) {DOWN = true;}
    else if (key == GLUT_KEY_RIGHT) {RIGHT = true;}
    else if (key == GLUT_KEY_LEFT) {LEFT = !LEFT;}

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void releaseKey(int key, int xx, int yy) { // Clear values when arrow keys are released
    if (key == GLUT_KEY_UP) {UP = false;}
    else if (key == GLUT_KEY_DOWN) {DOWN = false;}
    else if (key == GLUT_KEY_RIGHT) {RIGHT = false;}

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    readFile();
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(resolution, resolution);
    //set window position
    glutInitWindowPosition(100, 100);
    
    //create and set main window title
    MainWindow = glutCreateWindow("Proj5");
    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
    //sets display function
    glutDisplayFunc(display);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    // GetFile();
    PixelBuffer = new float[resolution*resolution*3];
    createmenu();
    glutMainLoop();//main display loop, will display until terminate
    return 0;

}
