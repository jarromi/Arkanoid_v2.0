/*
Class: platform
platform object is controlled by the user and allows to bounce the ball
*/
#ifndef _PLATFORM_
#define _PLATFORM_

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

// We define the boundaries between the platform can move
#ifndef _X_BNDR
#define _X_BNDR 10.0
#define _Y_BNDR 10.0
#endif

#include "../shader/shader.h"

class platform {
	static const float vertices[];			// vertices of the model
	static const unsigned int indices[];	// indices of the model (EBO)
	static unsigned int TextureID;			// texture of the model
	static unsigned int VAO;				// VAO index
	static unsigned int EBO;				// EBO index
	static unsigned int VBO;				// VBO index
	static unsigned int count;				// count of number of objects

	glm::vec3 position;						// position
	glm::vec3 lwh;							// (half) length, width and height - related to xscale
	float xscale;							// how to scale the graphic model in x direction (for later, bonuses)
	glm::mat4 modelMatrix;					// model matrix
	glm::vec3 color;						// colors storgae for different players
	int direction;							// direction in which the platform moves
	float TimeModif;						// time of last update of position
	float DeltaModif;						// average time between modifications
public:

	//constructor, destructor, assignment
	platform();
	platform(const float&);	// create with predefined x position
	platform(const float&, const glm::vec3 &);	// create with predefined x position and color
	platform(const platform&);// copy constructor
	~platform();
	platform& operator = (const platform&);

	// setters and getters
	void set_position(const float&);
	void set_xscale(const float&);
	void set_color(const glm::vec3&);
	glm::vec2 get_position() const;
	float get_xscale() const;
	glm::vec3 get_lwh() const;
	int get_direction() const;
	float get_TimeModif() const;
	float get_DeltaModif() const;

	// other methods
	void change_position(const float&);		// updates position based on mouse movement input
	void prepare_to_draw(const Shader&);
	void draw(const Shader&);

	float* comm_props(float*, float*);
	float* read_props(float*, float*);
};

#endif //_PLATFORM_
