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
#include "../stb_image.h"
#endif

#include "../shader/shader.h"

// We define the boundaries between the ball can move
#define _X_BNDR 9.5
#define _Y_BNDR 9.5

class ball {
	static const float vertices[];			// vertices of the model
	static const unsigned int indices[];	// indices of the model (for EBO)
	static const glm::vec3 color;			// Color of the ball
	static const glm::vec3 scaleVec;		// Scale of the ball
	static const glm::vec3 rotationAxis;	// Axis of rotation of the ball
	static unsigned int TextureID;			// texture of the model
	static unsigned int VAO;				// Verex Array Object ID
	static unsigned int EBO;				// Element Buffer Object ID
	static unsigned int VBO;				// Verex Buffer Object ID
	static unsigned int count;				// Counter of objects
	glm::mat4 modelMatrix;					// Model Matrix
public:
	static const float rad;					// Radius
	glm::vec3 velocity;						// Velocity (normalized!)
	float speed;							// Speed
	glm::vec3 position;						// Position
	glm::vec3 futurePosition;				// Position in the next frame

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
	float* comm_props(float*, float*);
};

#endif
