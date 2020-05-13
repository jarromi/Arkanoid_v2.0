#ifndef _BRICK_
#define _BRICK_

#include <iostream>
#include <fstream>

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

// We define the boundaries between the platform can move
#ifndef _X_BNDR
#define _X_BNDR 10.0
#define _Y_BNDR 10.0
#endif

#include "../shader/shader.h"

class brick {

	static const float vertices[];			// vertices of the model (for the VBO, OpenGL)
	static const unsigned int indices[];	// indices of the model (for the EBO, OpenGL)
	static const glm::vec3 colors[];		// colors for different states of a brick
	static unsigned int TextureID;			// texture id of the model
	static unsigned int VAO;				// vertex array object id
	static unsigned int EBO;				// element buffer object id
	static unsigned int VBO;				// vertex buffer object id
	static unsigned int count;				// number of objects

	glm::mat4 modelMatrix;					// model matrix		
	unsigned int state;						// state (how many hits to destroy)
	glm::vec3 position;						// position of a brick
public:
	static const glm::vec3 lwh;				// (half) length, width and height of the brick (const so can be public)


	//constructor, destructor, assignment operator
	brick();
	brick(const glm::vec2&, const unsigned int &);
	brick(const brick &);
	~brick();
	brick& operator = (const brick &);
	
	// getters and setters
	void set_position(const glm::vec2&);
	void set_state(const unsigned int&);
	glm::vec2 get_position() const;
	unsigned int get_state() const;
	static unsigned int get_count();

	// other methods
	void prepare_to_draw(const Shader&);
	void draw(const Shader &);
	void decrease_state();
	float* comm_props(float*, float*);
	float* read_props(float*, float*);
};

#endif
