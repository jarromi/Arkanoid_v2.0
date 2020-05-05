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

#include "../shader/shader.h"
#include "../brick/brick.h"
#include "../background/background.h"
#include "../platform/platform.h"
#include "../ball/ball.h"
#include "../bonus/bonus.h"

class level {
public:
	std::vector<brick> bricks;
	background _background;
	std::vector<ball> balls;
	std::vector<bonus> bonuses;
	unsigned int grade;
	bool end_level;
	int score;
	bool win_cond;

	level();
	~level();
	
	void load_level(const unsigned int&);
	void level_process_input(GLFWwindow*);

	void handle_bricks(const Shader&);								// handles bounces of bricks and draws bricks	
	void handle_bonuses(const Shader&, platform&, const float &);	// handles bonuses	
	void handle_balls(const Shader &, const float &);				// handles balls movement on screen

	bool check_bounce_brick(const int&, const  int&);	// checks for ball bounce off brick
	void check_bounce_platform(const platform&);		// checks for ball bounce off platform
	bool check_bonus(const int&, platform&);						// checks for bonus gain

	void check_lose();
	void clean_level();
};

#endif