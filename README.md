# WarAndPeace
Plane Fight game using OpenGL and C++. This is a simple game where the player has to shoot the opponent. There is also dual player mode. The animation of armstice is also done using OpenGL transformation functions.

## Basic Controls
 PLAYER 1                 
- a - to move left      
- d - to move right     
- w/SPACE - to shoot    
 PLAYER 2:
- <- - to move left
- -> - to move right
- left mouse click - to shoot

Dependencies : freeglut, stb

For Fedora : yum install freeglut-devel

For Ubuntu : sudo apt install freeglut3 freeglut3-dev

cmd to build(for linux) : make -f Makefile

cmd to build(for windows): make -f makefile.win

## Concepts used
- Here I have used gluPerspective for the projection matrix which helps to see the near objects bigger and far objects smaller.

- Terrain is implemented using GL_TRIANGLE_STRIP and choosing random z value for the appearance of uneven surface. Textures have been added for realistic view. To Load the image to texture stb_image library is used. 

- OpenGL timer functions are used to update the positions to the objects in the scene and to provide slow movement to the objects. The C++ STL i.e lists are used to store the co ordinates of the objects in the scene.

- GLLight is used to give lighting and shading to the scene. By using lighting we can change the theme from day to night and vice versa. GL_AMBIENT, GL_SPECULAR and GL_DIFFUSE are used to set the type to material which behave differently to light.

- freeglut is window creating library which is used to handle window related options. It is used to create menus, add KeyboardFunc and MouseFunc. 

![cg2](https://user-images.githubusercontent.com/32369302/58040396-0ea97780-7b53-11e9-8cb9-959385ea8d88.PNG)
Here you can choose dual mode or single mode.

![cg3](https://user-images.githubusercontent.com/32369302/58040397-0f420e00-7b53-11e9-8cd8-e713a1ba3494.PNG)
If you clicked War the above screen appears. You can right click to start the game.

![cg4](https://user-images.githubusercontent.com/32369302/58040398-0f420e00-7b53-11e9-9f8d-f63d832f4d39.PNG)
If you clicked peace the above scene appears.

![cg5](https://user-images.githubusercontent.com/32369302/58040399-0f420e00-7b53-11e9-80d9-708fab7414cb.PNG)
This is the final scene.
