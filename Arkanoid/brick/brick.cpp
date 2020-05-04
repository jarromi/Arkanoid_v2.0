#include "brick.h"

//initialize static variables
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
const glm::vec3 brick::colors[] = {
	glm::vec3(0.6784f, 0.6902f, 0.4118f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};
unsigned int brick::count = 0;
bool brick::needsTextureLoad = true;
bool brick::needsVAO = true;
unsigned int brick::TextureID=0;
unsigned int brick::VAO=0;
unsigned int brick::EBO=0;
unsigned int brick::VBO=0;
const glm::vec3 brick::lwh = glm::vec3(1.0f, 0.5f, 0.5f);

//default constructor
brick::brick() {
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
		data = stbi_load("./brick.png", &width, &height, &nrChannels, 0);	// load texture
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
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	state = 0;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

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

//positional constructor
brick::brick(const glm::vec2& _position, const unsigned int& _state) {
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
		data = stbi_load("./brick.png", &width, &height, &nrChannels, 0);	// load texture
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
	position = glm::vec3(_position,0.0f);
	state = _state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

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
brick::brick(const brick& _lhs) {
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
		data = stbi_load("./brick.png", &width, &height, &nrChannels, 0);	// load texture
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
	state = _lhs.state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

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
brick::~brick() {
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

//assignment operator
brick& brick::operator = (const brick& _lhs)
{
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
		data = stbi_load("./brick.png", &width, &height, &nrChannels, 0);	// load texture
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
	state = _lhs.state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

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
	return *this;
}

void brick::set_position(const glm::vec2& _position, const unsigned int& _state) {
	position = glm::vec3(_position,0.0f);
	state = _state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
}

void brick::prepare_to_draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindVertexArray(VAO);
}

void brick::draw(const Shader& _SO) {
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, colors[state].x, colors[state].y, colors[state].z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

unsigned int brick::get_count() {
	return count;
}
