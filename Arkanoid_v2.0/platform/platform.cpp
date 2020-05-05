#include "platform.h"

// -------------------------------------------------
// Initialization of static variables static variables

	// Since the model of a brick is fairly simple we define here all of its vertices
const float platform::vertices[] = {
	//back side
		-2.0f, -0.25f, -0.5f, 0.0f, 0.0f,
		 2.0f, -0.25f, -0.5f, 1.0f, 0.0f,
		 2.0f,  0.25f, -0.5f, 1.0f, 1.0f,
		-2.0f,  0.25f, -0.5f, 0.0f, 1.0f,
	//bottom side
		-2.0f, -0.25f,  2.0f, 0.0f, 0.0f,
		 2.0f, -0.25f,  2.0f, 1.0f, 0.0f,
		 2.0f, -0.25f, -2.0f, 1.0f, 1.0f,
		-2.0f, -0.25f, -2.0f, 0.0f, 1.0f,
	//right side
		 2.0f, -0.25f, -2.0f, 0.0f, 0.0f,
		 2.0f, -0.25f,  2.0f, 1.0f, 0.0f,
		 2.0f,  0.25f,  2.0f, 1.0f, 1.0f,
		 2.0f,  0.25f, -2.0f, 0.0f, 1.0f,
	 //top side
		-2.0f,  0.25f, -2.0f, 0.0f, 0.0f,
		 2.0f,  0.25f, -2.0f, 1.0f, 0.0f,
		 2.0f,  0.25f,  2.0f, 1.0f, 1.0f,
		-2.0f,  0.25f,  2.0f, 0.0f, 1.0f,
	 // left side
		-2.0f, -0.25f,  2.0f, 0.0f, 0.0f,
		-2.0f, -0.25f, -2.0f, 1.0f, 0.0f,
		-2.0f,  0.25f, -2.0f, 1.0f, 1.0f,
		-2.0f,  0.25f,  2.0f, 0.0f, 1.0f,
	 //front side
		-2.0f, -0.25f,  2.0f, 0.0f, 0.0f,
		 2.0f, -0.25f,  2.0f, 1.0f, 0.0f,
		 2.0f,  0.25f,  2.0f, 1.0f, 1.0f,
		-2.0f,  0.25f,  2.0f, 0.0f, 1.0f,
};

	// We draw the object using vertices combined into triangles.
	// Indices specify which indices to combine into which triangle.
const unsigned int platform::indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7,
		8, 9, 11,
		9, 10, 11,
		12, 13, 15,
		13, 14, 15,
		16, 17, 19,
		17, 18, 19,
		20, 21, 23,
		21, 22, 23
};
unsigned int platform::TextureID = 0;
unsigned int platform::VAO = 0;
unsigned int platform::EBO = 0;
unsigned int platform::VBO = 0;
unsigned int platform::count = 0;

// --------------------------------------------------------
// Constructors, destructors and assignment operator
	// Default constructor
platform::platform() {
	// keep count of number of bricks
	++count;

	// initialize the position, size etc.
	position = glm::vec3(0.0f, -9.0f, 0.0f);
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	xscale = 1.0f;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;	// arbitrary large value at the beggining

	// load textures if it is the first instance of an object 
	if (count == 1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./platform/platform.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// set VAO, EBO, VBO if needed

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

	// Positional constructor
platform::platform(const float &_xPos) {
	// keep count of number of bricks
	++count;
	// set the position and state
	float xPos = _xPos;
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	if (xPos < -_X_BNDR + lwh.x) xPos = -_X_BNDR + lwh.x;
	else if (xPos > _X_BNDR - lwh.x) xPos = _X_BNDR - lwh.x;
	position = glm::vec3(xPos, -9.0f, 0.0f);
	xscale = 1.0f;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;

	// load textures if first instance
	if (count == 1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./platform/platform.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// set VAO, EBO, VBO if needed
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//positional and color constructor
platform::platform(const float& _xPos, const glm::vec3 &_color) {
	// keep count of number of bricks
	++count;
	// set the position and state
	float xPos = _xPos;
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	if (xPos < -_X_BNDR + lwh.x) xPos = -_X_BNDR + lwh.x;
	else if (xPos > _X_BNDR - lwh.x) xPos = _X_BNDR - lwh.x;
	position = glm::vec3(xPos, -9.0f, 0.0f);
	xscale = 1.0f;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
	color = _color;
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;

	// load textures if first instance
	if (count == 1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./platform/platform.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// set VAO, EBO, VBO if needed
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//copy constructor
platform::platform(const platform& _lhs) {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = _lhs.position;
	lwh = _lhs.lwh;
	xscale = _lhs.xscale;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
	color = _lhs.color;
	direction = _lhs.direction;
	TimeModif = _lhs.TimeModif;
	DeltaModif = _lhs.DeltaModif;
}

//default destructor
platform::~platform() {
	--count;
	if (count == 0) {	// free the resources if this was the last instance
		glDeleteTextures(1, &TextureID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
}

//assignment operator
platform& platform::operator = (const platform& _lhs)
{
	// set the position and state
	position = _lhs.position;
	lwh = _lhs.lwh;
	xscale = _lhs.xscale;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
	color = _lhs.color;
	direction = _lhs.direction;
	TimeModif = _lhs.TimeModif;
	DeltaModif = _lhs.DeltaModif;

	return *this;
}

// -------------------------------------------------------------------
// Setters and getters
	// Set arbitrary position
void platform::set_position(const float& _xPos) {
	float xPos = _xPos;
	if (xPos < -_X_BNDR + lwh.x) xPos = -_X_BNDR + lwh.x;
	else if (xPos > _X_BNDR - lwh.x) xPos = _X_BNDR - lwh.x;
	position = glm::vec3(xPos,-9.0f,0.0f);
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
}

	// Set color
void platform::set_color(const glm::vec3& _color) {
	color = _color;
}

	// Set xscale
void platform::set_xscale(const float& _xscale) {
	xscale = _xscale;
	if (xscale > 2.0f) xscale = 2.0f;
	else if (xscale < 0.5f) xscale = 0.5f;
	lwh = glm::vec3(2.0f*xscale, 0.25f, 2.0f);
	if (position.x < -_X_BNDR + lwh.x) position.x = -_X_BNDR + lwh.x;
	else if (position.x > _X_BNDR - lwh.x) position.x = 10.0f - lwh.x;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
}

	// Get position
glm::vec2 platform::get_position() const {
	return glm::vec2(position.x, position.y);
}

// Get position
float platform::get_xscale() const {
	return xscale;
}

// Get position
glm::vec3 platform::get_lwh() const {
	return lwh;
}

// Get TimeModif
int platform::get_direction() const {
	return direction;
}

// Get TimeModif
float platform::get_TimeModif() const {
	return TimeModif;
}

// Get DeltaModif
float platform::get_DeltaModif() const {
	return DeltaModif;
}

// ------------------------------------------------------------------------------
// Other methods
	// Update the position based on mouse input
void platform::change_position(const float& xShift) {
	if (xShift > 0) direction = 1;
	else direction = -1;
	position.x += xShift;
	if (position.x < -_X_BNDR + lwh.x) {
		position.x = -_X_BNDR + lwh.x;
		direction = 0;
	}
	else if (position.x > _X_BNDR - lwh.x) {
		position.x = _X_BNDR - lwh.x;
		direction = 0;
	}
	else {
		float CurrTime = glfwGetTime();
		DeltaModif = (CurrTime - TimeModif) * 0.5 + DeltaModif * 0.5;	// average time between changes
		TimeModif = CurrTime;		// update time of last change to current
	}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
}

void platform::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

void platform::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, color.x, color.y, color.z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
