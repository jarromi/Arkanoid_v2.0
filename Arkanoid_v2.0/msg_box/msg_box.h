/*
Class: msg_box
For displaying message on the screen
*/
#ifndef _MSG_BOX_
#define _MSG_BOX_

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
#include "../logger/logger.h"

class msg_box {
	static const float vertices[];					// vertices of the model
	static const unsigned int indices[];			// indices of the model
	static unsigned int TextureID;				// textures of the model
	static unsigned int count;
	static unsigned int VAO;
	static unsigned int EBO;
	static unsigned int VBO;
	glm::mat4 modelMatrix;
	bool message_on;
public:
	unsigned int state;

	//constructor, destructor
	msg_box();
	~msg_box();

	// methods
	void set_state(const unsigned int&);
	void set_message_on();
	void set_message_off();
	void draw(const Shader&);
};

#endif _MSG_BOX_
