# Ray Tracer

---

Name: Daniel Andrade

---

### Overview

**Overview**
![Overview](imgs/Proj5(1).png)

**Menu**

![Menu](imgs/Proj5(Menu).png)

Using the arrow keys one can move around the spheres & ellispoids across the screen.
However, the graphics can look chopy because of high recursion levels and resolution
understanding of the scene.
Moving is the only input which involves the arrow keys, all other inputs prompt you to use the 
command line.
A maximum of 10 sphers/ellispoids, 10 planes, and 10 light sources can be used withing the scene,
along with an ambient light source
When an object is bright and having high k values or close to the light source, 
then other object can look dim. 

### Running the File

To run the program simply type "make" in the terminal
Then run the program by typing "main" in the terminal
The program will open and read from the provided text file called "" and then draw the scene.
While running the user can right click on the screen to bring up the menu.
The user can then select the action they wish to execute.
One can click the EXIT to exit the program and it will be stored in the same file.

### Text File Outline

The text file "raytrace.txt" is provided to load all pertinent info
That includes the scene dimensions, angles, and all objects in the scene

Attached is a sample text file with explanation. // Comments are not actually included in the text file.

```
500	            //Resolution of scene (both height and width)
45	            //Scene angle (how far from base viewing vector) the higher->more fisheye
2	            //Recursion level. The higher this is, the slower the program   
0 0 0	            //From Point (x y z)
1 0 0	            // At Point
0 0 1	            //Up Vector 
0.5 0.5 0.5	    //Ambient light (R G B)
3	            //Number of light sources
1 3 2 3 3 2	    //First light source location and color (x y z r g b)
2 -4 0 2 2 2	    //Second light source (x y z r g b)
1 -3 -1 5 5 5       //Third light source (x y z r g b)
2	            //Number of planes
1 1 1 1      	    //Definition of 1st plane (a b c d) => Ax + By + Cz = D
4 -4 1 4 1 -4 2 1 1	//Points on 1st plane (x1 y1 z1, x2 y2 z2, x3 y3 z3)
1 0.8 1 0.7 0.7 0.7 1 0.8 1 1	//K values for phong (Ka (RGB), Kd (RGB), Ks (RGB), alpha)
1 0 0 3	            //Definition of 2nd plane
3 -0.5 -0.5 3 -4 -3 3 -3 -4
0.8 0.9 1 0.8 1 0.9 0.8 1 0.8 3
1	            //Number of Ellipsoids
5.5 0.3 0.6 0.4 1 0.7 //Location and definition of 1st (x, y, z, a, b, c)
1 0.3 0.4 1 0.9 0.5 1 0.5 0.2 2 //K values for Phong
```

This file is rewritten at the closing of the program

### The Program

A baseray vector is made with the from and at points. (display() in main.cpp)
This is crossed with the up vector to get a right vector
The basery and right vector are recrossed with each other to get a guarenteed perpendicular up vector.
Then all vectors are normalized

THe angle of the 3 vectors on the screen are combined with sine and cosine in order to make a normalized ray (RayTrace() in main)
The function hitcheck() in main.cpp lines 342-372 and Nearhit() in lines 110-157 in Raytrace.cpp check if the ray hits.

The closest object it hits becomes the new surface. The phong model is applied there, then two new rays (reflection and refraction) are sent out recursively

Ambient light is applied to the function using the phong() lines 374-405 in main.cpp
It then tests the light source using the hitcheck() function to see if there is an object
If there is no obstruction, the diffuse and specular values are included. 

Once all the rays are calculated, the values are normalized and the screen displays. Larger resolutions and recursions mean longer frame times (easily a few seconds per frame)

### Popup Menu

press the RIGHT MOUSE BUTTON to activate the menu
Arrow keys are used to move the ellipsoid:
- press DOWN ARROW to move in the x direction
- press RIGHT ARROW to move in the y direction
- press UP ARROW to move in the z direction
- tap the LEFT ARROW to invert the direction of movement
- 
The user can choose to have a new From/At point or Up vector:
In the command line the user can input "0 1 2" would represent a point/vector of (0, 1, 2)
The user can choose a new angle:
	Input the value into the command line "50" would mean 50 degrees of visibity in both directions from the base viewing vector
The exit opetion will save the scene and close the program

### The Ellipsoid

A ellipsoid or sphere has the ability to move in the program.
The definition of an ellipsoid is:
	$$((x-x1)*a)^2 + ((y-y1)*b)^2 + ((z-z1)*c)^2 = 1$$
So the user must specify the base coordinates and a,b,c values
The smaller the a,b,c values, the longer the ellipsoid will be in that direction

### The Plane

One can view a plane if it is provided in the text file and defined as:
$$ (Ax + By + Zz = D) $$
Then the three points must be added to that plane.
**NOTE:** Currently it is unable to detect if points are on the plane, and if the points are incorrect
the program may crash.
All values are between 0 and 1, except for the alpha which should be a whole number greater than 0
Then the plane is defined by its K values
That is broken down into the Ambient, Diffuse, and Specular values for RGB, as well as the alpha value. 






