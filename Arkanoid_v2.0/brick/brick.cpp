#include "brick.h"

// -------------------------------------------------
// Initialization of static variables static variables

	// Since the model of a brick is fairly simple we define here all of its vertices
const float brick::vertices[] = {
	//back side
		-1.0f, -0.5f, -0.5f, 0.0f, 0.0f,
		 1.0f, -0.5f, -0.5f, 1.0f, 0.0f,
		 1.0f,  0.5f, -0.5f, 1.0f, 1.0f,
		-1.0f,  0.5f, -0.5f, 0.0f, 1.0f,
	//bottom side
		-1.0f, -0.5f,  0.5f, 0.0f, 0.0f,
		 1.0f, -0.5f,  0.5f, 1.0f, 0.0f,
		 1.0f, -0.5f, -0.5f, 1.0f, 1.0f,
		-1.0f, -0.5f, -0.5f, 0.0f, 1.0f,
	//right side
		 1.0f, -0.5f, -0.5f, 0.0f, 0.0f,
		 1.0f, -0.5f,  0.5f, 1.0f, 0.0f,
		 1.0f,  0.5f,  0.5f, 1.0f, 1.0f,
		 1.0f,  0.5f, -0.5f, 0.0f, 1.0f,
	 //top side
		 -1.0f,  0.5f, -0.5f, 0.0f, 0.0f,
		  1.0f,  0.5f, -0.5f, 1.0f, 0.0f,
		  1.0f,  0.5f,  0.5f, 1.0f, 1.0f,
		 -1.0f,  0.5f,  0.5f, 0.0f, 1.0f,
	 // left side
		 -1.0f, -0.5f,  0.5f, 0.0f, 0.0f,
		 -1.0f, -0.5f, -0.5f, 1.0f, 0.0f,
		 -1.0f,  0.5f, -0.5f, 1.0f, 1.0f,
		 -1.0f,  0.5f,  0.5f, 0.0f, 1.0f,
	//front side
		 -1.0f, -0.5f,  0.5f, 0.0f, 0.0f,
		  1.0f, -0.5f,  0.5f, 1.0f, 0.0f,
		  1.0f,  0.5f,  0.5f, 1.0f, 1.0f,
		 -1.0f,  0.5f,  0.5f, 0.0f, 1.0f,
};

	// We draw the object using vertices combined into triangles.
	// Indices specify which indices to combine into which triangle.
const unsigned int brick::indices[] = {
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

	// We define 3 main colors of the bricks
	// Brick color will represent its state - how many hits needed to destroy
const glm::vec3 brick::colors[] = {
	glm::vec3(0.1718f, 0.664f, 0.844),
	glm::vec3(0.844, 0.664f, 0.1718f),
	glm::vec3(0.1718f, 0.664f, 0.1718f)
};

	// Textures and vertex array object (VAO), element buffer object (EBO)
	// and vertex buffer object (VBO) are shared between all of the objects
	// of type "brick"
	// Here are ids of these objects
unsigned int brick::TextureID = 0;
unsigned int brick::VAO = 0;
unsigned int brick::EBO = 0;
unsigned int brick::VBO = 0;

	// We keep count of the number of brick objects through count
unsigned int brick::count = 0;

	// Physical dimensions of the brick are stored in lwh
const glm::vec3 brick::lwh = glm::vec3(1.0f, 0.5f, 0.5f);

// -------------------------------------------------
// Constructors and destructors
	// Default constructor, loads graphics only if needed
brick::brick() {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	state = 0;	// state: {0 -> 1 hit, 1 -> 2 hits, 2 -> 3 hits, etc.}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);


	// load textures if this is the first object
	if (count==1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./brick/brick.png", &width, &height, &nrChannels, 0);	// load texture using stbi
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			logger::log("Failed to load the texture.\n");
			throw "BAD_BRICK";
		}
		stbi_image_free(data);	// release the memory of the image
	}
	// set VAO, EBO, VBO if needed
	if (count==1) {
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

	// Positional constructor - sets a brick at a specified location with specified state
	// Similar to default constructor
brick::brick(const glm::vec2& _position, const unsigned int& _state) {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = glm::vec3(_position, 0.0f);
	if (position.x > _X_BNDR - lwh.x) {
		position.x = _X_BNDR - lwh.x;
	}
	else if (position.x < -_X_BNDR + lwh.x) {
		position.x = -_X_BNDR + lwh.x;
	}
	if (position.y > _Y_BNDR - lwh.y) {
		position.y = _Y_BNDR - lwh.y;
	}
	else if (position.y < -_Y_BNDR + lwh.y) {
		position.y = -_Y_BNDR + lwh.y;
	}
	state = _state;
	if (state > 3) { state = 3; }
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);


	// load textures if neede
	if (count==1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./brick/brick.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			logger::log("Failed to load the texture.\n");
			throw "BAD_BRICK";
		}
		stbi_image_free(data);	// release the memory of the image
	}
	// set VAO, EBO, VBO if needed
	if (count == 1) {
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

	// Copy constructor
	// Doesn't need texture loading, because if it is called another object of this type already exists
brick::brick(const brick& _lhs) {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = _lhs.position;
	state = _lhs.state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

}

	// Default destructor
	// If the last object is destroyed, then free the resources
brick::~brick() {
	--count;
	if (count == 0) {
		glDeleteTextures(1, &TextureID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
}

// ------------------------------------------------------------
// Assignment operator
brick& brick::operator = (const brick& _lhs)
{
	// set the position and state
	position = _lhs.position;
	state = _lhs.state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

	return *this;
}

// -------------------------------------------------------------
// Setters and getters
	// setter position
void brick::set_position(const glm::vec2& _position) {
	position = glm::vec3(_position,0.0f);
	if (position.x > _X_BNDR - lwh.x) {
		position.x = _X_BNDR - lwh.x;
	}
	else if (position.x < -_X_BNDR + lwh.x) {
		position.x = -_X_BNDR + lwh.x;
	}
	if (position.y > _Y_BNDR - lwh.y) {
		position.y = _Y_BNDR - lwh.y;
	}
	else if (position.y < -_Y_BNDR + lwh.y) {
		position.y = -_Y_BNDR + lwh.y;
	}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
}

	// setter state
void brick::set_state(const unsigned int &_state) {
	state = _state;
	if (state > 3) { state = 3; }
}

	// getter position
glm::vec2 brick::get_position() const {
	return glm::vec2(position.x,position.y);
}

	// getter position
unsigned int brick::get_state() const {
	return state;
}

	// Get the count of bricks
unsigned int brick::get_count() {
	return count;
}

// -------------------------------------------------------------
// Other class methods

	//Prepare OpenGL to draw object of this type
void brick::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

	// Tells OpenGL to draw object of this type
void brick::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, colors[state].x, colors[state].y, colors[state].z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

	// Decreases state by 1
void brick::decrease_state() {
	if (state > 0) state--;
}

	// Prepares data for communication with other users
	// The data that should be communicated are position and state
	// _lptr - points to the memory position where to write data
	// _rptr - points to the memory position beyond which we cannot write
	// returns pointer to the first free address after writing
float* brick::comm_props(float* _lptr, float*_rptr) {
	if (_lptr + 3 <= _rptr) {
		*_lptr = position.x;
		_lptr += 1;
		*_lptr = position.y;
		_lptr += 1;
		*_lptr = (float)state;
		_lptr += 1;
		return _lptr;
	}
	else{
		logger::log("Not enough space for data communication.\n");
		throw "BAD_BRICK_COMM";
	}
}

// Prepares data for communication with other users
// The data that should be communicated are position and state
// _lptr - points to the memory position where to write data
// _rptr - points to the memory position beyond which we cannot write
// returns pointer to the first free address after writing
float* brick::read_props(float* _lptr, float* _rptr) {
	if (_lptr + 3 <= _rptr) {
		position.x = *_lptr;
		_lptr += 1;
		position.y = *_lptr;
		_lptr += 1;
		state = (int)*_lptr;
		_lptr += 1;
		modelMatrix = glm::mat4(1.0);
		modelMatrix = glm::translate(modelMatrix, position);
		return _lptr;
	}
	else {
		logger::log("Not enough space for data communication.\n");
		throw "BAD_BRICK_COMM";
	}
}