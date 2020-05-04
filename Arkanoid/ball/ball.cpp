#include "ball.h"

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

const glm::vec3 ball::color = glm::vec3(0.3f,0.3f,0.3f);

unsigned int ball::TextureID=0;			// texture of the model

bool ball::needsTextureLoad = true;

unsigned int ball::count = 0;

bool ball::needsVAO = true;

unsigned int ball::VAO = 0;
unsigned int ball::EBO = 0;
unsigned int ball::VBO = 0;

const float ball::rad = 0.5f;

glm::vec3 ball::scaleVec = glm::vec3(0.2628655, 0.2628655, 0.2628655);

//default constructor
ball::ball() {
	// keep count of number of bricks
	++count;

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			needsTextureLoad = false;
		}
		else {
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image
	}

	// set the position and state
	position = glm::vec3(0.0f, -8.25f, 0.0f);
	futurePosition = position;
	velocity = glm::vec3(0.0f, 1.0f, 0.0f);
	speed = 0.0f;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);


	// set VAO, EBO, VBO if needed
	if (needsVAO) {
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
		needsVAO = false;
	}
}

//position constructor
ball::ball(const glm::vec2 &_pos, const glm::vec2 &_vel, const float &_spd) {
	// keep count of number of bricks
	++count;

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
			needsTextureLoad = false;
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image
	}

	// set the position and state
	position = glm::vec3(_pos, 0.0f);
	futurePosition = position;
	velocity = glm::normalize(glm::vec3(_vel, 0.0f));
	speed = _spd;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);

	// set VAO, EBO, VBO if needed
	if (needsVAO) {
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
		needsVAO = false;
	}
}

//copy constructor
ball::ball(const ball &_lhs) {
	// keep count of number of balls
	++count;

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			needsTextureLoad = false;
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image
	}

	// set the position and state
	position = _lhs.position;
	futurePosition = _lhs.futurePosition;
	velocity = _lhs.velocity;
	speed = _lhs.speed;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);

	// set VAO, EBO, VBO if needed
	if (needsVAO) {
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
		needsVAO = false;
	}
}

//assignment operator
ball& ball::operator = (const ball& _lhs) {

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./white.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
			needsTextureLoad = false;
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image
	}

	// set the position and state
	position = _lhs.position;
	futurePosition = _lhs.futurePosition;
	velocity = _lhs.velocity;
	speed = _lhs.speed;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);

	// set VAO, EBO, VBO if needed
	if (needsVAO) {
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
		needsVAO = false;
	}
}

//default destructor
ball::~ball() {
	--count;
	if (count == 0) {
		if (!needsTextureLoad) {
			needsTextureLoad = true;
			glDeleteTextures(1, &TextureID);
		}
		if (!needsVAO) {
			needsVAO = true;
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}
	}
}

void ball::set_position(const glm::vec2& _pos)
{
	position = glm::vec3(_pos, 0.0f);
	futurePosition = position;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);
}


void ball::set_velocity(const glm::vec2 &_vel) {
	velocity = glm::normalize(glm::vec3(_vel, 0.0f));
}

void ball::set_speed(const float& _spd) {
	speed = _spd;
}

void ball::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

void ball::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, color.x, color.y, color.z);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
}

void ball::propagate(const float &deltaTime){
	position += speed*velocity * deltaTime;
	futurePosition = position + speed * velocity * deltaTime;
	if (position.x > 9.5f) {
		position.x = 9.5f;
		velocity.x *= -1.0f;
	}
	else if (position.x < -9.5f) {
		position.x = -9.5f;
		velocity.x *= -1.0f;
	}
	if (position.y > 9.5f) {
		position.y = 9.5f;
		velocity.y *= -1.0f;
	}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scaleVec);
}

bool ball::out_of_bounds() {
	if (position.y < -10.0f) return true;
	else return false;
}