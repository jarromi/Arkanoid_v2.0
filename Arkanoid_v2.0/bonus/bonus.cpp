#include "bonus.h"

// -------------------------------------------------
// Initialization of static variables static variables

	// Since the model is fairly simple we define here all of its vertices
const float bonus::vertices[] = {
	//back side
		-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
		 0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
		 0.25f,  0.25f, -0.25f, 1.0f, 1.0f,
		-0.25f,  0.25f, -0.25f, 0.0f, 1.0f,
	//bottom side
		-0.25f, -0.25f,  0.25f, 0.0f, 0.0f,
		 0.25f, -0.25f,  0.25f, 1.0f, 0.0f,
		 0.25f, -0.25f, -0.25f, 1.0f, 1.0f,
		-0.25f, -0.25f, -0.25f, 0.0f, 1.0f,
	//right side
		 0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
		 0.25f, -0.25f,  0.25f, 1.0f, 0.0f,
		 0.25f,  0.25f,  0.25f, 1.0f, 1.0f,
		 0.25f,  0.25f, -0.25f, 0.0f, 1.0f,
	 //top side
		 -0.25f,  0.25f, -0.25f, 0.0f, 0.0f,
		  0.25f,  0.25f, -0.25f, 1.0f, 0.0f,
		  0.25f,  0.25f,  0.25f, 1.0f, 1.0f,
		 -0.25f,  0.25f,  0.25f, 0.0f, 1.0f,
	 // left side
		 -0.25f, -0.25f,  0.25f, 0.0f, 0.0f,
		 -0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
		 -0.25f,  0.25f, -0.25f, 1.0f, 1.0f,
		 -0.25f,  0.25f,  0.25f, 0.0f, 1.0f,
	//front side
		 -0.25f, -0.25f,  0.25f, 0.0f, 0.0f,
		  0.25f, -0.25f,  0.25f, 1.0f, 0.0f,
		  0.25f,  0.25f,  0.25f, 1.0f, 1.0f,
		 -0.25f,  0.25f,  0.25f, 0.0f, 1.0f,
};

	// We draw the object using vertices combined into triangles.
	// Indices specify which indices to combine into which triangle.
const unsigned int bonus::indices[] = {
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

	// We define 3 main colors of the bonuses
	// Bonus color will represent its state
const glm::vec3 bonus::colors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};

	// Textures and vertex array object (VAO), element buffer object (EBO)
	// and vertex buffer object (VBO) are shared between all of the objects of type "bonus"
	// Here are ids of these objects
unsigned int bonus::TextureID = 0;
unsigned int bonus::VAO = 0;
unsigned int bonus::EBO = 0;
unsigned int bonus::VBO = 0;

	// We keep count of the number of bonuses objects through count
unsigned int bonus::count = 0;

	// Physical dimensions of the bonus are stored in lwh
const glm::vec3 bonus::lwh = glm::vec3(0.25f, 0.25f, 0.25f);

// -------------------------------------------------
// Constructors and destructors
	// Default constructor
bonus::bonus() {
	// keep count of number of bonuses
	++count;

	// set the position and state
	position = glm::vec3(0.0f, 0.0f, 0.25f);
	state = 0;	// state: {0 -> polatform wide, 1 -> double balls, 2 -> platform narrow, etc.}
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
		data = stbi_load("./bonus/white.png", &width, &height, &nrChannels, 0);	// load texture using stbi
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			logger::log("Failed to load the bonus texture.\n");
			throw "BAD_BONUS";
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

	// Positional constructor - sets a bonus at a specified location with specified state
	// Similar to default constructor
bonus::bonus(const glm::vec2& _position, const unsigned int& _state) {
	// keep count of number of bonuses
	++count;

	// set the position and state
	position = glm::vec3(_position, 0.25f);
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
		data = stbi_load("./bonus/white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			logger::log("Failed to load the bonus texture.\n");
			throw "BAD_BONUS";
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
bonus::bonus(const bonus& _lhs) {
	// keep count of number of bonuses
	++count;

	// set the position and state
	position = _lhs.position;
	state = _lhs.state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

}

	// Default destructor
	// If the last object is destroyed, then free the textures, VAO, VBO, and EBO
bonus::~bonus() {
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
bonus& bonus::operator = (const bonus& _lhs)
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
void bonus::set_position(const glm::vec2& _position) {
	position = glm::vec3(_position, 0.25f);
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
void bonus::set_state(const unsigned int &_state) {
	state = _state;
	if (state > 3) { state = 3; }
}

	// getter position
glm::vec2 bonus::get_position() const {
	return glm::vec2(position.x,position.y);
}

	// getter position
unsigned int bonus::get_state() const {
	return state;
}

	// Get the count of bonuses
unsigned int bonus::get_count() {
	return count;
}

// -------------------------------------------------------------
// Other class methods

	//Prepare OpenGL to draw object of this type
void bonus::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

	// Tells OpenGL to draw object of this type
void bonus::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, colors[state].x, colors[state].y, colors[state].z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

	// Prepares data for communication with other users
	// The data that should be communicated are position, state
	// _lptr - points to the memory position where to write data
	// _rptr - points to the memory position beyond which we cannot write
	// returns pointer to the first free address after writing
float* bonus::comm_props(float* _lptr, float*_rptr) {
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
		logger::log("Not enough space for data.\n");
		throw "BAD_BONUS_COMM";
	}
}

// Prepares data for communication with other users
// The data that should be communicated are position, state
// _lptr - points to the memory position where to read data from
// _rptr - points to the memory position beyond which we cannot read
// returns pointer to the first free address after writing
float* bonus::read_props(float* _lptr, float* _rptr) {
	if (_lptr + 3 <= _rptr) {
		position.x = *_lptr;
		_lptr += 1;
		position.y = *_lptr;
		_lptr += 1;
		state = (int)*_lptr;
		_lptr += 1;
		return _lptr;
	}
	else {
		logger::log("Not enough space for data.\n");
		throw "BAD_BONUS_COMM";
	}
}

	// Propagates bonus down
void bonus::propagate(const float& deltaTime) {
	if (position.y > -20.0f) {
		position.y -= 3. * deltaTime;
		modelMatrix = glm::mat4(1.0);
		modelMatrix = glm::translate(modelMatrix, position);
	}
}

	// Checks if bonus is out of bounds
bool bonus::out_of_bounds() {
	if (position.y < -10.0f) return true;
	else return false;
}