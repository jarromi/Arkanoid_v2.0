#ifndef _BALL_
#define _BALL_

#define _PHI_ICOSAHEDRON 1.618034

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

class ball {
	static const float vertices[];			// vertices of the model
	static const unsigned int indices[];		// indices of the model
	static const glm::vec3 color;		// colors for different levels
	static unsigned int TextureID;			// texture of the model
	static bool needsTextureLoad;
	static unsigned int count;
	static bool needsVAO;
	static unsigned int VAO;
	static unsigned int EBO;
	static unsigned int VBO;
	static glm::vec3 scaleVec;
	glm::mat4 modelMatrix;
public:
	glm::vec3 velocity;
	float speed;
	glm::vec3 position;
	glm::vec3 futurePosition;
	static const float rad;

	//constructor, destructor, assignment
	ball();
	ball(const glm::vec2&, const glm::vec2&, const float &);
	ball(const ball&);
	~ball();
	ball& operator = (const ball&);

	// functionalities
	void set_position(const glm::vec2&);
	void set_velocity(const glm::vec2&);
	void set_speed(const float&);
	void prepare_to_draw(const Shader&);
	void draw(const Shader&);
	void propagate(const float&);
	bool out_of_bounds();
};

#endif
