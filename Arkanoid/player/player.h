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

#include "platform.h"

class player {
	static player* currentOwner;
	static bool firstMouseAction;
	double lastXpos;
public:
	player();
	~player();
	platform plat;

	// functionalities
	void draw(const Shader&);
	static void mouse_callback(GLFWwindow*, double, double);
};

#endif
