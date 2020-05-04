#include "platform.h"

//initialize static variables
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
unsigned int platform::count = 0;
bool platform::needsTextureLoad = true;
bool platform::needsVAO = true;
unsigned int platform::VAO = 0;
unsigned int platform::EBO = 0;
unsigned int platform::VBO = 0;

//default constructor
platform::platform() {
	// keep count of number of bricks
	++count;
	xscale = 1.0f;

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		needsTextureLoad = false;
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./platform.png", &width, &height, &nrChannels, 0);	// load texture
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

	// set the position and state
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;
	position = glm::vec3(0.0f, -9.0f, 0.0f);
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale,1.0f,1.0f));

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
platform::platform(const float &_xPos) {
	// keep count of number of bricks
	++count;
	xscale = 1.0f;

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
		data = stbi_load("./platform.png", &width, &height, &nrChannels, 0);	// load texture
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
	float xPos = _xPos;
	if (xPos < -8.0f) xPos = -8.0f;
	else if (xPos > 8.0f) xPos = 8.0f;
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;
	position = glm::vec3(xPos,-9.0f,0.0f);
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));

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

//positional and color constructor
platform::platform(const float& _xPos, const glm::vec3 &_color) {
	// keep count of number of bricks
	++count;
	xscale = 1.0f;

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
		data = stbi_load("./platform.png", &width, &height, &nrChannels, 0);	// load texture
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
	float xPos=_xPos;
	if (xPos < -8.0f) xPos = -8.0f;
	else if (xPos >  8.0f) xPos =  8.0f;
	direction = 0;
	TimeModif = glfwGetTime();
	DeltaModif = 1.0f;
	position = glm::vec3(xPos, -9.0f, 0.0f);
	lwh = glm::vec3(2.0f, 0.25f, 2.0f);
	color = _color;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));

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
platform::platform(const platform& _lhs) {
	// keep count of number of bricks
	++count;
	xscale = _lhs.xscale;

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
		data = stbi_load("./platform.png", &width, &height, &nrChannels, 0);	// load texture
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
	direction = _lhs.direction;
	TimeModif = _lhs.TimeModif;
	DeltaModif = _lhs.DeltaModif;
	position = _lhs.position;
	lwh = _lhs.lwh;
	color = _lhs.color;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));

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
platform::~platform() {
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
platform& platform::operator = (const platform& _lhs)
{
	// keep count of number of bricks
	xscale = _lhs.xscale;

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
		data = stbi_load("./platform.png", &width, &height, &nrChannels, 0);	// load texture
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
	direction = _lhs.direction;
	TimeModif = _lhs.TimeModif;
	DeltaModif = _lhs.DeltaModif;
	position = _lhs.position;
	lwh = _lhs.lwh;
	color = _lhs.color;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));

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

void platform::set_position(const float& _xPos) {
	float xPos = _xPos;
	if (xPos < -8.0f) xPos = -8.0f;
	else if (xPos > 8.0f) xPos = 8.0f;
	position = glm::vec3(xPos,-9.0f,0.0f);
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
}

void platform::change_position(const float& xPos) {
	if (xPos > 0) direction = 1;
	else direction = -1;
	position.x += xPos;
	if (position.x < -10.0f + lwh.x) {
		position.x = -10.0f + lwh.x;
		direction = 0;
	}
	else if (position.x > 10.0f - lwh.x) {
		position.x = 10.0f - lwh.x;
		direction = 0;
	}
	else {
		DeltaModif = (glfwGetTime() - TimeModif) * 0.5 + DeltaModif * 0.5;
		TimeModif = glfwGetTime();
	}
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xscale, 1.0f, 1.0f));
}

void platform::set_color(const glm::vec3& _color) {
	color = _color;
}

void platform::set_xscale(const float& _xscale) {
	xscale = _xscale;
	lwh = glm::vec3(2.0f*xscale, 0.25f, 2.0f);
	if (position.x < -10.0f + lwh.x) position.x = -10.0f + lwh.x;
	else if (position.x > 10.0f - lwh.x) position.x = 10.0f - lwh.x;
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
