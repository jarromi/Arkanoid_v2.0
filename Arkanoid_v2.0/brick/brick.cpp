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

	// Textures and vertex array object (VAO), element buffer object and vertex buffer object are shared between all of the objects of type "brick"
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
	// Default constructor
brick::brick() {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	state = 0;	// state: {0 -> 1 hit, 1 -> 2 hits, 2 -> 3 hits, etc.}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);	// sets the object at appropriate position visually


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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// pass data to OpenGL
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
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
	state = _state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);


	// load textures if neede
	if (count==1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./brick/brick.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
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
	// If the last object is destroyed, then free the textures, VAO, VBO, and EBO
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
// Other class methods
	// Setting position and state of a brick with specific values
void brick::set_position(const glm::vec2& _position, const unsigned int& _state) {
	position = glm::vec3(_position,0.0f);
	state = _state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
}

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

	// Get the count of bricks
unsigned int brick::get_count() {
	return count;
}

	// Prepares data for communication with other users
	// The data that should be communicated are state and count -- if code missed change of state for one player
	// _lptr - points to the memory position where to write data
	// _rptr - points to the memory position beyond which we cannot write
	// returns pointer to the first free address after writing
int* brick::comm_props(int* _lptr, int *_rptr) {
	if (_lptr + 2 <= _rptr) {
		*_lptr = state;
		_lptr += 1;
		*_lptr = count;
		_lptr += 1;
		return _lptr;
	}
	else{
		std::cout << "Not enough space for data.\n";
		return _lptr;
	}
}