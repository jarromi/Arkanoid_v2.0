#ifndef _LEVEL_
#define _LEVEL_

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>

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

#include "shader.h"
#include "brick.h"
#include "background.h"
#include "platform.h"
#include "player.h"
#include "ball.h"

class level {
public:
	std::vector<brick> bricks;
	player _player;
	background _background;
	ball _ball;
	unsigned int grade;
	bool end_level;
	int score;
	bool win_cond;

	level();
	
	void load_level(const unsigned int&);
	void play_level(GLFWwindow*, Shader &);
	void level_process_input(GLFWwindow*);

	bool check_bounce_brick(const brick&, ball&);
	void check_bounce_platform(const platform&, ball&);

	void check_lose(const ball&);
};

#endif