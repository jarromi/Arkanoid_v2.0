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

#ifndef _STB_IMAGE_JP_
#define _STB_IMAGE_JP_
#include "../stb_image.h"
#endif

#include "../shader/shader.h"

#include "../brick/brick.h"
#include "../background/background.h"
#include "../platform/platform.h"
#include "../player/player.h"
#include "../ball/ball.h"
#include "../level/level.h"

#include "singleplayer.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

int singleplayer() {
	//	--------------------------------------------------------------------------------------------------------------------
// Here starts window and context initialization
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid_v0.1", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to open window.\n";
		return 1;
	}
	// Finally we make the context of the window the main context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// now try to initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD.\n";
		return 1;
	}

	//	--------------------------------------------------------------------------------------------------------------------
	// data communication channel is set now take care of shaders with a special object
	Shader SO("./shader/vertexShader.vert", "./shader/fragmentShader.frag");
	SO.use();

	glEnable(GL_DEPTH_TEST);


	cout << "Starting game.\n";
	level _level;
	_level.load_level(1);
	_level.play_level(window, SO);
	cout << "                                        \r" << "You scored: " << _level.score << endl;
	if (_level.win_cond) {
		_level.win_cond = false;
		_level.load_level(2);
		_level.play_level(window, SO);
		cout << "                                        \r" << "You scored: " << _level.score << endl;
	}

	//at the end we want to properly clean the resources
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}