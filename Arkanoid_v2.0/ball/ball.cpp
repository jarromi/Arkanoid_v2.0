#include "ball.h"

// -------------------------------------------------------------
// Initialization of static variables
	// Vertices (in header file since only a few of them)
const float ball::vertices[] = {
	 0.0f, 1.0f, _PHI_ICOSAHEDRON, 0.1f, 0.2f,
	 0.0f, 1.0f,-_PHI_ICOSAHEDRON, 0.8f, 0.4f,
	 0.0f,-1.0f, _PHI_ICOSAHEDRON, 0.3f, 0.2f,
	 0.0f,-1.0f,-_PHI_ICOSAHEDRON, 0.6f, 0.4f,
	 //
	  1.0f, _PHI_ICOSAHEDRON, 0.0f, 0.0f, 0.4f,
	  1.0f,-_PHI_ICOSAHEDRON, 0.0f, 0.4f, 0.4f,
	 -1.0f, _PHI_ICOSAHEDRON, 0.0f, 0.9f, 0.2f,
	 -1.0f,-_PHI_ICOSAHEDRON, 0.0f, 0.5f, 0.2f,
	 //
	  _PHI_ICOSAHEDRON, 0.0f, 1.0f, 0.2f, 0.4f,
	  _PHI_ICOSAHEDRON, 0.0f,-1.0f, 0.5f, 0.6f,
	 -_PHI_ICOSAHEDRON, 0.0f, 1.0f, 0.6f, 0.0f,
	 -_PHI_ICOSAHEDRON, 0.0f,-1.0f, 0.7f, 0.2f
};

	// Indices for the EBO (in header file since only a few of them)
const unsigned int ball::indices[] = {
	0, 2, 8,
	0, 4, 6,
	0, 6, 10,
	0, 8, 4,
	0, 10, 2,
	1, 3, 9,
	1, 4, 6,
	1, 6, 11,
	1, 9, 4,
	1, 11, 3,
	2, 5, 7,
	2, 7, 10,
	2, 8, 5,
	3, 5, 7,
	3, 7, 11,
	3, 9, 5,
	4, 8, 9,
	5, 8, 9,
	6, 10, 11,
	7, 10, 11
};

	// Color - shade of grey
const glm::vec3 ball::color = glm::vec3(0.3f,0.3f,0.3f);
const glm::vec3 ball::scaleVec = glm::vec3(0.2628655, 0.2628655, 0.2628655);
const glm::vec3 ball::rotationAxis = glm::vec3(1.0f, 1.0f, 0.1f);

	//Texture, VAO, EBO, and VBO indices
unsigned int ball::TextureID=0;	
unsigned int ball::VAO = 0;
unsigned int ball::EBO = 0;
unsigned int ball::VBO = 0;

unsigned int ball::count = 0;

const float ball::rad = 0.5f;

// ---------------------------------------------------------------------------------
// Constructors, destructors and assignment operator
	// Default constructor
ball::ball() {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = glm::vec3(0.0f, -8.25f, 0.0f);	// Initial position at the bottom of the screen
	futurePosition = position;					// Set the position in the next frame to be the same as initial
	velocity = glm::vec3(0.0f, 1.0f, 0.0f);		// Velocity - by default straight up
	speed = 0.0f;								// Speed - initially 0
	modelMatrix = glm::mat4(1.0);				// modelMatrix will map the object for drawing
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);



	// load textures if neede
	if (count == 1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./ball/white.png", &width, &height, &nrChannels, 0);	// load texture - plain white
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
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

	// Position constructor - set position, velocity and speed
ball::ball(const glm::vec2 &_pos, const glm::vec2 &_vel, const float &_spd) {
	// keep count of number of bricks
	++count;

	// set the position and state
	position = glm::vec3(_pos, 0.0f);
	if (position.x > _X_BNDR - rad) {
		position.x = _X_BNDR - rad;
	}
	else if (position.x < -_X_BNDR + rad) {
		position.x = -_X_BNDR + rad;
	}
	if (position.y > _Y_BNDR - rad) {
		position.y = _Y_BNDR - rad;
	}
	else if (position.y < -_Y_BNDR + rad) {
		position.y = -_Y_BNDR + rad;
	}
	futurePosition = position;
	velocity = glm::normalize(glm::vec3(_vel, 0.0f));
	if (abs(velocity.y) < abs(velocity.x) * 0.3f) {
		float sign = velocity.y / abs(velocity.y);
		velocity.y = abs(velocity.x) * 0.3f * sign;
		velocity = glm::normalize(velocity);
	}
	speed = _spd;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);

	// load textures if neede
	if (count == 1) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./ball/white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	
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

	//Copy constructor
ball::ball(const ball &_lhs) {
	// keep count of number of balls
	++count;

	// set the position and state
	position = _lhs.position;
	futurePosition = _lhs.futurePosition;
	velocity = _lhs.velocity;
	speed = _lhs.speed;
	modelMatrix = _lhs.modelMatrix;

	// No need to load textures VAO, VBO, EBO
}

	// Assignment operator
ball& ball::operator = (const ball& _lhs) {

	// set the position and state
	position = _lhs.position;
	futurePosition = _lhs.futurePosition;
	velocity = _lhs.velocity;
	speed = _lhs.speed;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);

	// No need to load textures VAO, VBO, EBO

	return *this;
}

//default destructor
ball::~ball() {
	--count;
	if (count == 0) {
		glDeleteTextures(1, &TextureID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
}

// -----------------------------------------------------------------------
// Setters and getters
	// Set position
void ball::set_position(const glm::vec2& _pos)
{
	position = glm::vec3(_pos, 0.0f);
	if (position.x > _X_BNDR - rad) {
		position.x = _X_BNDR - rad;
	}
	else if (position.x < -_X_BNDR + rad) {
		position.x = -_X_BNDR + rad;
	}
	if (position.y > _Y_BNDR - rad) {
		position.y = _Y_BNDR - rad;
	}
	else if (position.y < -_Y_BNDR + rad) {
		position.y = -_Y_BNDR + rad;
	}
	futurePosition = position;
	float angle = glfwGetTime()*1000.f;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle),rotationAxis);
}

	// Set velocity
void ball::set_velocity(const glm::vec2 &_vel) {
	velocity = glm::normalize(glm::vec3(_vel, 0.0f));
	if (abs(velocity.y) < abs(velocity.x) * 0.3f) {
		float sign = velocity.y / abs(velocity.y);
		velocity.y = abs(velocity.x) * 0.3f * sign;
		velocity = glm::normalize(velocity);
	}
}

	// Set speed
void ball::set_speed(const float& _spd) {
	speed = _spd;
}

	// Get position
glm::vec2 ball::get_position() const {
	return glm::vec2(position.x, position.y);
}
	// Get futurePosition
glm::vec2 ball::get_future_position() const {
	return glm::vec2(futurePosition.x, futurePosition.y);
}

	// Get velocity
glm::vec2 ball::get_velocity() const {
	return glm::vec2(velocity.x,velocity.y);
}

	// Get speed
float ball::get_speed() const {
	return speed;
}

	// Get count
unsigned int ball::get_count() {
	return count;
}

// -----------------------------------------------------------------------
// Other methods and functions
	// Prepare to draw
void ball::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

	// Draw an object
void ball::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, color.x, color.y, color.z);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
}

	// Propagate with a time discretization deltaTime
void ball::propagate(const float &deltaTime){
	position += speed*velocity * deltaTime;
	float angle = glfwGetTime() * 1000.f;
	futurePosition = position + speed * velocity * deltaTime;
	if (position.x > _X_BNDR-rad) {
		position.x = _X_BNDR-rad;
		velocity.x *= -1.0f;
	}
	else if (position.x < -_X_BNDR+rad) {
		position.x = -_X_BNDR+rad;
		velocity.x *= -1.0f;
	}
	if (position.y > _Y_BNDR-rad) {
		position.y = _Y_BNDR-rad;
		velocity.y *= -1.0f;
	}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), rotationAxis);
}

	// Condition for loosing
bool ball::out_of_bounds() {
	if (position.y < -10.0f) return true;
	else return false;
}

	// Function for network communication
	// We need to exchange information about about:
	// velocity, position, futurePosition, speed, and count
	// _lptr - points to the memory position where to write data
	// _rptr - points to the memory position beyond which we cannot write
	// returns pointer to the first free address after writing
float* ball::comm_props(float* _lptr, float* _rptr) {
	if (_lptr + 8 <= _rptr) {
		//velocity
		*_lptr = velocity.x;
		_lptr += 1;
		*_lptr = velocity.y;
		_lptr += 1;
		//position
		*_lptr = position.x;
		_lptr += 1;
		*_lptr = position.y;
		_lptr += 1;
		//futurePosition
		*_lptr = futurePosition.x;
		_lptr += 1;
		*_lptr = futurePosition.y;
		_lptr += 1;
		//speed
		*_lptr = speed;
		_lptr += 1;
		//count
		*_lptr = (float)count;
		_lptr += 1;
		return _lptr;
	}
	else {
		std::cout << "Not enough space in the given pointer.\n";
		return _lptr;
	}
}