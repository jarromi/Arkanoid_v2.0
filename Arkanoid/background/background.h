#ifndef _BACKGROUND_
#define _BACKGROUND_

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
#include "stb_image.h"
#endif

#include "shader.h"

class background {
	static const float vertices[];			// vertices of the model
	static const unsigned int indicesFloor[];		// indices of the model
	static const unsigned int indicesCeiling[];		// indices of the model
	static const unsigned int indicesWalls[];		// indices of the model
	static unsigned int TextureID[];			// textures of the model
	static unsigned int count;
	static bool needsTextureLoad;
	static bool needsVAO;
	static unsigned int VAO[];
	static unsigned int EBO[];
	static unsigned int VBO;
	glm::mat4 modelMatrix;
public:
	unsigned int state;

	//constructor, destructor, assignment
	background();
	background(const unsigned int &);
	~background();

	// functionalities
	void set_state(const unsigned int&);
	void draw(const Shader&);
};

#endif _BACKGROUND_
