# The Phong Lighting Model, Gouraud Shading, Half-Toning, and The Painters Algorithm

---

Name: Daniel Andrade 

---

### Overview

**Output**

![Output](imgs/Proj3(3).png)

**Halftone**

![Halftone](imgs/Proj3(Halftone).png)

**Phong**

![Phong](imgs/Proj3(Phong).png)

A maximum of 5 shapes may be displayed. All of the variables in the **Phong** equation are editable via textfile (test.txt) or menu
Shapes and Coordinates can also have their own color, which affects their display (A purely red object will not reflect blue or green light)
One can Rotate or Translate shapes with the arrow keys
The color of vertices is determined using the normal vectors of the plane being rendered.
Only triangles can be rendered.
The projections are scaled to fit all images in the window
The Light point (X) and the From point (O) are shown on the screen
No gl() functions were used besides those required to display my PIXELBUFFER and the text shown in the bottom right screen.

### Running the Program

Compile the makefile by typing "make" then run it by typing "main"
The program reads the data from the test.txt and draws to 3 windows

- The BOTTOM LEFT window is the XY projection
- The TOP LEFT is the XZ projection
- The TOP RIGHT is the YZ projection
- The menu is activated in the BOTTOM RIGHT corner

### Textfile Outline

The textfile is read in the program, then writes to it in closing


The following is a walkthrough of a sample text file (one is provided), **NOTE:** the // comments are not actually present

```
2                   //number of shapes
4                   //Number of vertices in shape 1
-1 -1 0.5 1 0 0     //The x,y,z coordinates of vertex 1, and the r,g,b values of that vertex
-1 0.5 1 1 1 1
0.5 1 1.5 1 1 1
1 0.5 2 1 1 1
4                   //Number of Triangles in shape 1
1 2 3               //Triangle 1 connects vertex 1, 2, and 3
1 2 4
1 3 4
2 3 4
6                   //Number of vertices in shape 2
4 1.5 3 0 0 1       //vertex values
3.49 3.37 1.23 0 1 0
2.29 1.95 1.2 1 0 0
1.77 1.39 2.33 1 1 1
3 2.8 3 1 1 1
1.61 3.168 1.6 1 1 1
8                   //Triangles in shape 2
1 2 3
4 5 6
1 2 5
2 3 6
1 3 4
4 5 1
5 6 2
4 6 3
-0.5 2 0.1 1 1 1    //Light x,y,z position and r,g,b values
1.3 1 0.5           //From point location x,y,z
0.5 0.5 0.5         //Ambient Light r,g,b values
```


### Display/Painters Algorithm

The painters algorithm is located in the SORT files.
The algorithm is refered to as PaintOrder in main
It refers to the Order function in Coord.h 54-111

The algorithm uses topological sorting
All the triangles make a list of all triangles that must be rendered before them
depth is determined by size. A large z will be rendered before a small z in the XY window

DeleteMax returns a triangle that does not have any other triangles that must be returned first
Every Single Triangle is rendered

### Gouraud Shading

The Phong function calculates values at each vertex. Gouroud shading calculates the values at all other rendered points
I adjusted my Bresenham line drawing function from Project 1
It reads the light values between the two points, and shades a gradient between the two points
It is contained in lines 271-472 in main.cpp


### Popup Menu

Use the RIGHT MOUSE BUTTON to bring up the menu
click on the BOTTOM RIGHT corner of the screen
From this menu it is possible to change shape coordinates, light values, and phong values


### Shape Locations

Select MOVE SHAPE from the menu, then which shape to move, and finally whether you would like to rotate or translate
Either selection requires input from the UP, DOWN, LEFT, and RIGHT BUTTONS

##### Translating

- The LEFT BUTTON reverses the direction of movement
- To translate the shape press the DOWN BUTTON to move in the X direction
- The RIGHT BUTTON to move in the Y direction
- The UP BUTTON to move in the Z direction
	
##### Rotating

- To Rotate the shape press the DOWN BUTTON to rotate about the X AXIS
- The RIGHT BUTTON to rotate about the Y AXIS
- The UP BUTTON to rotate about the Z AXIS

### Phong Values

Ka, Ks, Kd, and n are set to 1 by default
To change the Phong values (Ka, Ks, Kd, n) select PHONG in the popup menu
Select the Shape to change (A shape shares all the same values)
The "reccomended" values to input will be given on the command line (along with the input)

The Phong Algorithm is in Sort.cpp along lines 217 - 292

### Light Vector

Light is seen as an X on the screen
Light is from ambient light and from the light point
Select LIGHT from the Pop-up menu
if the option says (input) it will be command line
if it says (arrow keys) it is using the arrow keys
If you are changing the location of the light vector using arrow keys:
- The LEFT BUTTON reverses the direction of movement
- The DOWN BUTTON to move in the X direction
- The RIGHT BUTTON to move in the Y direction
- The UP BUTTON to move in the Z direction

If you are changing the light values using arrow keys:
- The LEFT BUTTON reverses the change of value
- The DOWN BUTTON to adjust the RED value
- The RIGHT BUTTON to adjust the GREEN value
- The UP BUTTON to adjust the BLUE value

### From Point

The from point is also refered to as where the viewer is located, and is seen as a O
To change the values select VIEWER in the popup menu
To change the coordinates, either choose input or arrow keys
Refer to THE LIGHT VECTOR for instructions on how to adjust coordinates

### Coordinate Color

It is not possible to add new coordinates during runtime. However it is possible to shade the coordinates
Select COORDINATE COLOR in the popup menu
On the command line type the shape number the coordinate belongs to
Then select the exact coordinate (refer to test.txt to see what the values are)
Then input the red, green, and blue values for the given coordinate

### Halftone

To make the display purely binary select HALFTONE in the menu
Halftoning averages the pixel values for a set of nine pixels (3x3)
Select Halftone again to go back to the normal display

The Halftoning algorithm is located in Sort.cpp in lines 309-454

### Exiting

To save coordinate locations and light vector values, select EXIT in the popup menu
This saves the file to test.txt and quits the program
