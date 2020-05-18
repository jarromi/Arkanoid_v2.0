# Arkanoid v2.0
### 18.05.2020
### Jaromir Panas

#### Introduction

This repository has been created for the solution of a recruitment task for
a position of C++ developer. The task reads:

"A Breakout (eg. Arkanoid) game. Use DirectX (9,10,11,12), OpenGL or Vulkan
and the fact that the scene is 3D should be visible. No flat sprites. May be
ugly (programmer's art) though.

The important part is to make 2 (or more?) persons Multiplayer. Not just 2
inputs but 2 computers and real-time multiplayer. Both players play on the
same map. So no different games followed by score comparison. Gameplay should
be synced on both machines. Can be Co-Op or PvP.

No Unity, Unreal or Middleware. Plain simple C++ and sockets. May be ugly
and simple (but no hardwired IP addresses)."

I have chosen to implement an Arkanoid game, with co-op for two players.
I have been working on Windows platform with Visual Studio Community 2019
and haven't test the code on other programming environments. In this repository
I include:
1) files with the source code of a working version,
2) .sln file with Visual Studio solution,
3) .proj\* files with Visual Studio project.
I also include some headers and libraries for API's I have been using.
However, these may not work on other machines therefore an independent
download and setup of API's listed below is recommended.

#### Dependencies:

The code has been written for Windows platforms with winsock2.

The code has been written using Visual Studio Community so it is best to
run it in the same environments. E.g., if using CodeBlocks liking of winsock
libraries might look slightly different.

Apart form standard C++ libraries I have been working with quite a few other
APIs, mostly for the purpose of simplifying graphics programming with OpenGL.
Therefore, in order to run the game one needs:
1. GLFW3 - API for window, context creation in OpenGL: https://www.glfw.org/,
2. GLAD - Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on
the official specs: https://glad.dav1d.de/,
3. GLM - math library for OpenGL: https://glm.g-truc.net/0.9.9/index.html,
4. stb (actually just stbi_image.h header file) - image loading/decoding
from file/memory: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC. Source:
https://github.com/nothings/stb/,
5. winsock2 - a library for Windows for creating, connecting and communicating
servers, clients, etc. (included with Windows).

All of these are included either in this repository or with Windows
distribution. However, the first three were built on my system and issues might
arise when running them on other systems. It is recommended to download them
form the links provided above and build before running the program.

In the Visual Studio solution I have linked the above libraries based on paths
specific for my notebook. When running the program on other PCs the paths to
the linked libraries need to be appropriately modified. On VS2019 this can
be changed under: Project->Properties->VC++ Directories->Include/Library Dirs.

#### Issues

When running the game I have observed some issues:
1. Rarely (about 3% of the cases) the game crashed during multiplayer gameplay.
I believe this issue is related to the server-client communication and its
implementation with threads but I haven't resolved it yet. I have placed the
communication in separate thread and used blocking communication there.
Blocking mode was used since we only need to communicate single server with
a single client. It was placed in a different thread in order to avoid freezing
the gameplay when communication goes wrong.
2. Rarely (also about 3% of the cases) the games crashes when loading
multiplayer server/client. This issue also haven't been resolved.
3. Collision detection: bonus-platform. The game is designed in a way that
server should control the gameplay and client should only control his platform.
However, because of this solution client might miss catching bonus (e.g., when
bonus jumps due to update with data form server). Then client does not update
the state of his platform appropriately (no collision detected).
4. Collision detection: ball-brick. The current version handles ball-bounce
off the bricks edge differently than off the corner. Since collision detection
goes through the list of all bricks independently it will handle corner bounces
seperately for each brick. This might lead to weird behavior when the ball hits
the corner shared by two bricks.


#### To-do list:

Most relevant - resolving issues:
1. Solve the issues with unfrequent crashes. Possibly threading in here is
an overkill.
2. Solve the issues with unfrequent crashes during loading.
3. Improve the server-client communication - let the server handle collisions
between bonuses and both platforms. Client should control only postion of his
platform.
4. Imporve the ball-brick collision algorithm. Do not allow bounces into other
bricks or simplify the collisions.

Second priority - improve the gameplay and graphics:
1. For now only a single level is implemented with a single ball speed. Adding
levels with different bricks arrangement and ball speeds would be good.
2. This version follows simple Arcanoid gameplay without adding enything
original. It would be recommended to add new aspects to the gameplay. E.g., one
could leverage the fact that the game is multiplayer:
	- force the players to cooperate more by sharing the platform control
	between them,
	- adding several stages with view controled by mouse and platform controled
	by keyboard and forcing the players to play on several stages
	simultaneously,
	- ...
3. Enhance the graphics. Right now the game uses very simple models and
textures. It would be niceto add more detailed models, nicer textures, add
lighting effects, etc.
4. Remove the console aspect of the game. Because I haven't yet finished the 
text rendering in OpenGL for the game, the text has to be either loaded as a
texture or diplayed on the console. This is not the intended solution. When
text displaying support will be available the game should get its own start
screen, score counter, etc.

Last priority:
1. At this point the game has to be compiled using some C++ programming
evnironment. This is of course not ideal - especially with the external APIs.
In future it would be good to make a installer, that takes care of these steps
and generates easy to use executable.