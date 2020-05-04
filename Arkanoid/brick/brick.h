#ifndef _BRICK_
#define _BRICK_

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

class brick {
	static const float vertices[];			// vertices of the model
	static const unsigned int indices[];	// indices of the model
	static const glm::vec3 colors[];		// colors for different levels
	static unsigned int TextureID;			// texture id of the model
	static unsigned int count;				// number of objects
	static bool needsTextureLoad;			// bool check if texture loading is needed
	static bool needsVAO;					// bool check if VAO, EBO, VBO loading is needed
	static unsigned int VAO;				// vertex array object id
	static unsigned int EBO;				// element buffer object id
	static unsigned int VBO;				// vertex buffer object id
	glm::mat4 modelMatrix;					// model matrix
public:
	static const glm::vec3 lwh;				// (half) length, width and height of the brick
	unsigned int state;						// state (how many hits to destroy)
	glm::vec3 position;						// position

	//constructor, destructor, assignment
	brick();
	brick(const glm::vec2&, const unsigned int &);
	brick(const brick &);
	~brick();
	brick& operator = (const brick &);
	
	// functionalities
	void set_position(const glm::vec2&, const unsigned int &);
	void prepare_to_draw(const Shader&);
	void draw(const Shader &);
	static unsigned int get_count();
};

#endif
