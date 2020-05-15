/*
Singleplayer.cpp
File includes the definition of the singleplayer function.
This function is responsible for the singleplayer gameplay:
1) Creating window
2) Loading graphics
3) Calling functions for handling input, movement of objects, etc.
4) Closing everything and resetting after the end of the game.
*/

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
#include "../bonus/bonus.h"
#include "../level/level.h"
#include "../logger/logger.h"

#include "singleplayer.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

void play_level_single(GLFWwindow*, Shader&, level&, player&);

// A function that creates a context window and intializes gameplay for a single player
int singleplayer() {
	//	--------------------------------------------------------------------------------------------------------------------
	// Here starts window and context initialization
	logger::log("    Singleplayer: initializing window and graphics.\n");
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid_v2.0", NULL, NULL);
	if (window == NULL) {
		logger::log("Failed to open window.\n");
		return 1;
	}
	// Finally we make the context of the window the main context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// now try to initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger::log("Failed to initialize GLAD.\n");
		return 1;
	}

	//	--------------------------------------------------------------------------------------------------------------------
	// now take care of shaders with a special object
	logger::log("    Singleplayer: initializing shader.\n");
	Shader SO("./shader/vertexShader.vert", "./shader/fragmentShader.frag");
	SO.use();

	glEnable(GL_DEPTH_TEST);

	// Load level and initialize gameplay
	logger::log("    Singleplayer: loading level.\n");
	bonus dummy_bonus(glm::vec2(0.0f, -20.0f), 0);
	level _level;
	player _player;
	try {
		_level.load_level(1);
	}
	catch (const char* msg) {
		std::stringstream ss;
		ss << "Failed in level load " << msg;
		logger::log(ss);
		return 1;
	}
	logger::log("    Singleplayer: initializing gameplay.\n");
	play_level_single(window, SO, _level, _player);

	logger::log("    Singleplayer: cleaning up.\n");
	_player.resetMouseAction();
	_level.clean_level();


	//at the end we want to properly clean the resources
	glfwTerminate();

	logger::log("    Singleplayer: exiting.\n");
	return 0;
}

// Function for handling window resizing
void framebuffer_size_callback(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}

// Function for gameplay
void play_level_single(GLFWwindow *window, Shader &_SO, level &_level, player &_player) {
	// Initialize callbacks, view and projection
	logger::log("        Singleplayer play level: initializing callbacks and perspective.\n");
	glfwSetCursorPosCallback(window, player::mouse_callback);
	if (_level.bricks.size() > 0) _level.end_level = false;
	float deltaTime = 0.0f;
	float lastFrame = glfwGetTime();

	_SO.use();
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int tView = glGetUniformLocation(_SO.ID, "view");
	glUniformMatrix4fv(tView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	unsigned int tProj = glGetUniformLocation(_SO.ID, "proj");
	glUniformMatrix4fv(tProj, 1, GL_FALSE, glm::value_ptr(proj));


	// start rendering loop
	logger::log("        Singleplayer play level: starting rendering loop.\n");
	while (!_level.end_level) {
		_level.level_process_input(window);
		Sleep(10);

		float timeVal = glfwGetTime();
		deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen

		_level.message.draw(_SO);
		_level._background.draw(_SO);
		_level.handle_bricks(_SO);
		_level.handle_bonuses(_SO, _player.plat, deltaTime);
		_level.handle_balls(_SO, deltaTime);
		_level.check_bounce_platform(_player.plat);

		_player.plat.prepare_to_draw(_SO);
		_player.plat.draw(_SO);

		_level.check_lose();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	_player.resetMouseAction();

	logger::log("        Singleplayer play level: ended rendering loop; exit.\n");
}