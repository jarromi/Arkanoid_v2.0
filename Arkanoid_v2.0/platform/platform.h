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

#include "../shader/shader.h"

class platform {
	static const float vertices[];			// vertices of the model
	static const unsigned int indices[];		// indices of the model
	static unsigned int TextureID;			// texture of the model
	static unsigned int count;
	static bool needsTextureLoad;
	static bool needsVAO;
	static unsigned int VAO;
	static unsigned int EBO;
	static unsigned int VBO;
	glm::mat4 modelMatrix;
	glm::vec3 color;		// colors for different players
public:
	int direction;
	float TimeModif;		// time of last update of position
	float DeltaModif;		// average time between modifications
	glm::vec3 position;
	glm::vec3 lwh;			//(half) length, width and height
	float xscale;

	//constructor, destructor, assignment
	platform();
	platform(const float&);	// create with predefined x position
	platform(const float&, const glm::vec3 &);	// create with predefined x position and color
	platform(const platform&);// copy constructor
	~platform();
	platform& operator = (const platform&);

	// functionalities
	void set_position(const float&);
	void change_position(const float&);
	void set_color(const glm::vec3&);
	void set_xscale(const float&);
	void prepare_to_draw(const Shader&);
	void draw(const Shader&);
};

#endif //_PLATFORM_
