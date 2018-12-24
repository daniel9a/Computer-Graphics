# 3d Transformation and Projection System

----

Name:      Daniel Andrade

----

### Overiew

![alt-text](imgs/Proj2(2).png)

There is a 5 shape maximum, it is recomended to render at least 1 shape
All functions (translation, rotation, scaling) are animated
You may right click on bottom right of screen to open up a menu and change the settings of a specific shape.
Everything is done using the arrow keys:

- Up represents Z axis 
- Right represnts Y axis 
- Down represents X axis.
  
All input is with ARROW KEYS and MENUS, not the command line. (With exception to New File Command)
Line drawing is handled with glVertex()

### Running the Program

To run the program run the makefile using the command 'make'
Run the main program that is produced by that by simply typing "main"
You may right click on bottom right of screen to open up a menu and change the settings of a specific shape.
Everything is done using the arrow keys. Up represents Z axis, Right represnts Y axis, and Down represents X axis.
All input is with ARROW KEYS and MENUS, not the command line. (With exception to New File Command)

### Text File Details

##### OPENING:
By default the program opens "test.txt"
but if a different text file is desired
select the New File option in the menu. Then type the name of the Text file in the command line

##### FORMAT:
The basic format of the text file is take the number of shapes,
then for each shape take the number of vertices
    then each vertex coordinate (Floats or Ints are OK)
    then for each shape take the number of lines
        then each line (from coordinate "x" to coordinate "y")
	Then the last two lines are the initial coordinates for the rotation vector

This is the default text file, with a walkthrough (**NOTE**: comments are not actually in file)

```
3                             **Three Shapes**
4                             **First Shape has 4 vertices**
206.472 462.236 401.337       **1st Coordinate**
261.007 140.395 566.458       **2nd Coordinate...**
352.548 384.274 300.747
417.902 386.621 512.788
6                             **6 lines**
1 2                           **1st line is between coordinate 1 and 2 (Should be an INT value)**
1 3
1 4
2 3
2 4
3 4
8                            **Second Shape has 8 vertices**
-69.2123 679.442 -39.3466    **Coordinates**
196.594 582.577 -63.9248
95.795 319.404 -63.1805
-170.011 416.269 -38.6022
-52.8409 674.455 159.836
212.966 577.59 135.258
112.166 314.417 136.002
-153.64 411.282 160.58
12 
1 2
2 3
3 4
4 1
5 6
6 7
7 8
8 5
5 1
6 2
7 3
8 4
4                           **Third Shape has 4 lines**
-159.117 278.45 157.773     **Coordinates**
155.15 113.229 265.589
-58.0778 486.095 226.9
-257.349 267.744 378.562
6
1 2
1 3
1 4
2 3
2 4
3 4
0 0 1                       **First Coordinate of Rotation vector**
1 1 0                       **Second Coordinate**
```

### Display

There are 3 displays with images. The top left the XZ plane, top right is YZ and bottom left is XY.
The Bottom Right screen is used for the menu.
The subwindows are boxed to help display the regions
Each Shape has a slightly different color to help differentiate the shapes
The Rotation Vertex is also visible, with the vertex going from the blue side to the red (Direction matters)
Vertices are not specifically drawn, just the lines
    (So if there is a vertex that has no lines, it will not draw, but will affect Master values)

### Popup Menu

Right Click the bottom right hand part of the screen to have the menu popup.
The options are to rotate, translate, scale, rotation vector, new file, exit, or print vertices
The submenus pick which shape that function is being applied to
After Selecting an option use the ARROW KEYS to move the shape with the desired option.

#### Rotation

Select Rotate from the menu and which shape to rotate in the submenu

The shape rotates only a few degrees at a time, so hold down the button until it is in the desired position

The rotation vector goes from the blue side to the red side
algorithim in Shape.cpp at 42-77

#### Translation

Select Translate from the menu and which shape to translate
ALL TRANSLATIONS ARE DONE WITH ARROW KEYS!

PRESS THEN RELEASE THE LEFT KEY BEFORE PRESSING THE OTHER KEYS AGAIN. IF AT ANY TIME THE LEFT KEY IS PRESSED, THE DIRECTIONS WILL REVERSE
Even if you are currently in the Scale menu, the left key negates directions in translate
algorithm in Shape.cpp at 79-87

#### Scale

Select the Scale from the menu and the shape to scale
Shapes are scaled about the centroid of the shape
algorithm in Shape.cpp at 89-103

#### Rotation Vector

There are two options to choose in the menu, and they correspond to the two points of the vector
The first point is the blue side, second point is the red side
SEE TRANSLATION RULES FOR HOW TO MOVE THE ROTATION VECTORS (The rules are the same)
This is the default option when the program starts

#### New File

Selecting the new file command will prompt the user to input the name of the file containing the coordinates
This is the only time the command line will be used
if this option is taken on accident, enter 3dcoordinates.txt (The default file)
The command will then revert to whatever was last selected (rotate, translate, .e.g)

#### Print Vertices 

If you want to see the vertices for a specific shape, or the rotation vector
select this option then specify the shape to have the values printed in the command line

#### Exit

THIS QUITS THE PROGRAM
Choose this option in the popup window
The program will save all shapes and the rotation vector in the selected file

