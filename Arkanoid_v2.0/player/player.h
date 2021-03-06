/*
Class: player
This class connects the controls of the user with a specific platform
*/
#ifndef _PLAYER_
#define _PLAYER_

#include <iostream>

#ifndef _GLAD_
#define _GLAD_
#include <glad/glad.h>	// API for pointers in OpenGL
#endif
#ifndef _GLFW3_
#define _GLFW3_
#include <glfw3.h>		// API for context creation
#endif

#ifndef _GLM_STUFF_
#define _GLM_STUFF_
#include <glm/glm/glm.hpp>	// API for linear algebra operations compatible with OpenGL
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#endif

#include "../platform/platform.h"

class player {
	static player* currentOwner;	// points to the object of type player that is given the control over the mouse
	static bool firstMouseAction;	// checks if mouse has been moved before
	double lastXpos;				// last position of the cursor
public:
	player();
	player(const glm::vec3 &);		// Constructor with predefined color of the platform
	~player();
	platform plat;		// platform is made public in order to simplify its control with mouse input

	// functionalities
	static void resetMouseAction();
	static void mouse_callback(GLFWwindow*, double, double);
};

#endif
