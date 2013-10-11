Readme
======

This is the readme file for Edward Seabrook's (ejfs1g10) Computer Graphics
Coursework 3 Submission.

Controls
========

I have taken the metaphor of the "camera equipped oil drum" quite literally. The viewer is 
affected by gravity and collides with static objects and terrain. The elevation of the camera 
doesn't affect what it will collide with: it is like an extending pole with a camera at the top.

> Movement
  --------
Left and right turn the camera, this behavior repeats so can be held down. Up and down increase 
and decrease the acceleration, this does not repeat so must be tapped again. Page Up and Down increase 
and decrease the elevation. Space stops all motion of the viewer.

> Touring
  -------
The tour can be started using the T key, and stopped using the E key. During the tour no other controls 
will work. When the tour comes to and end the controls will be reactivated. 
Pressing P will move the user to the location the screen shot was taken at. 

<ESC> and Q can be used to quit. H can be used to show a help screen and FPS counter.
 
List of Files
=============

Each .cpp (except main.cpp)  file listed below also has a corresponding .hpp file. For brevity, 
I have ommitted descriptions of these files.

+ main.cpp - Contains the main method. 
	Sets up the scene and runs the draw loop.
	Handles the controls.
+ object.cpp - Represents an object in the scene. Contains most of the drawing code.
+ viewer.cpp - Represents the viewer of the scene. 
	Handles all collision detection and movement
+ material.cpp - Represents the material of an object. Includes location of a texture etc.
+ geometrygenerator.cpp - Abstract class specifying and interface to get Vertices, UVs, etc.
+ objloader.cpp - Used to generate an object a wavefront .obj file. Is-a GeometryGenerator
+ heightmaploader.cpp - Used to generate an object from a height map.
+ mtlloader.cpp - Used to cerate a list of Materials from an mtl file.
+ skybox.cpp - Represents the skybox in which the scene resides.
+ textgenerator.cpp - Used to print text to the OpenGL context.
+ animutator.cpp - Specifies how an object is animated.
+ tour.cpp - Specifies a route for the camera to take.
+ utils.cpp - Utilities provided by John Carter.

+ gl_common.hpp - Contains the imports that are used by  all files.

*.gls - These files are my shaders. (Using Unix line endings)
*.obj - These files are OBJ format models
*.mtl - These files are material files (colours, textures etc)
*.jpg & *.png - These are image files, usually textures

Building
========

I have been developing under Linux on ECS UGlabs machines. I use the makefile included.

How it Works
============

My program uses object oriented C++. My objects are loaded for OBJ files, with the materials
loaded from MTL files. Texture loading is performed using glImg. Objects may have multiple textures.  
The terrain is generated with a heightmap.

Collision with the terrain is done by finding the nearest vertex, and collision with static 
objects is done using bounding spheres.

Tours and animation are specified using a series of waypoints and keyframes respectively, stored with 
their corresponding times.  

Text is drawn using a font texture and a specialised shader. 

Inspiration
===========

All code included has been written by me. I have used ideas from http://opengl-tutorials.org. 
See credits.txt for complete credits listing.
