#include "background.h"

//initialize static variables
const float background::vertices[] = {
	//back side
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 5.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 5.0f, 10.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 10.0f,
	//bottom side
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 5.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 5.0f, 5.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 5.0f,
	//right side
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 5.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 5.0f, 10.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 10.0f,
	 //top side
		 -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	 // left side
		 -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f, 5.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 5.0f, 10.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 10.0f
};
const unsigned int background::indicesFloor[] = {
		4, 5, 7,
		5, 6, 7
};
const unsigned int background::indicesWalls[] = {
		0, 1, 3,
		1, 2, 3,
		8, 9, 11,
		9, 10, 11,
		16, 17, 19,
		17, 18, 19
};
const unsigned int background::indicesCeiling[] = {
		12, 13, 15,
		13, 14, 15
};
unsigned int background::TextureID[] = {0,0,0};
unsigned int background::count = 0;
bool background::needsTextureLoad = true;
bool background::needsVAO = true;
unsigned int background::VAO[] = { 0,0,0 };
unsigned int background::EBO[] = { 0,0,0 };
unsigned int background::VBO = 0;

//default constructor
background::background() {
	// keep count of number of bricks
	++count;
	if (count > 1) {
		std::cout << "Only one object of type 'background' was ever ment to be created.\n";
		throw(101);
	}
	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(3, TextureID);
		// load textrure of the floor
		glBindTexture(GL_TEXTURE_2D, TextureID[0]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		data = stbi_load("./floor.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// load textrure of the walls
		glBindTexture(GL_TEXTURE_2D, TextureID[1]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		data = stbi_load("./walls.png", &width, &height, &nrChannels, 0);	// load texture
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// load textrure of the ceiling
		glBindTexture(GL_TEXTURE_2D, TextureID[2]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		needsTextureLoad = false;
		data = stbi_load("./ceiling.png", &width, &height, &nrChannels, 0);	// load texture
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
	state = 1;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f,10.0f,10.0f));

	// set VAO, EBO, VBO if needed
	if (needsVAO) {
		glGenVertexArrays(3, VAO);
		glGenBuffers(3, EBO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesFloor), indicesFloor, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWalls), indicesWalls, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCeiling), indicesCeiling, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
background::background(const unsigned int& _state) {
	// keep count of number of bricks
	++count;
	if (count > 1) {
		std::cout << "Only one object of type 'background' was ever ment to be created.\n";
		throw(101);
	}

	// set the position and state
	state = _state;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));

	// load textures if neede
	if (needsTextureLoad) {
		glGenTextures(3, TextureID);
		// load textrure of the floor
		glBindTexture(GL_TEXTURE_2D, TextureID[0]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		unsigned char* data;
		int width, height, nrChannels;
		if (state == 1 || state == 2) {
			data = stbi_load("./floor.png", &width, &height, &nrChannels, 0);	// load texture
		}
		else {
			std::cout << "This type of background was not yet implemented.\n";
			throw(100);
		}
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// load textrure of the walls
		glBindTexture(GL_TEXTURE_2D, TextureID[1]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		if (state == 1 || state == 2) {
			data = stbi_load("./floor.png", &width, &height, &nrChannels, 0);	// load texture
		}
		else {
			std::cout << "This type of background was not yet implemented.\n";
			throw(100);
		}
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load the texture.\n";
			throw(100);
		}
		stbi_image_free(data);	// release the memory of the image

		// load textrure of the ceiling
		glBindTexture(GL_TEXTURE_2D, TextureID[2]);	// bind texture // next set properties of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat the mirrored texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// repeat the texture once fully through it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// downscaling of textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// downscaling of textures
		needsTextureLoad = false;
		if (state == 1 || state == 2) {
			data = stbi_load("./floor.png", &width, &height, &nrChannels, 0);	// load texture
		}
		else {
			std::cout << "This type of background was not yet implemented.\n";
			throw(100);
		}
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
	if (needsVAO) {
		glGenVertexArrays(3, VAO);
		glGenBuffers(3, EBO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesFloor), indicesFloor, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWalls), indicesWalls, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCeiling), indicesCeiling, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
background::~background() {
	--count;
	if (count == 0) {
		if (!needsTextureLoad) {
			needsTextureLoad = true;
			glDeleteTextures(3, TextureID);
		}
		if (!needsVAO) {
			needsVAO = true;
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(3, EBO);
			glDeleteVertexArrays(3, VAO);
		}
	}
}

void background::set_state(const unsigned int& _state) {
	state = _state;

	//reload the textures
	if (needsTextureLoad) {
		std::cout << "Something went wrong with texture loading.\n";
		throw(100);
	}
	unsigned char* data;
	int width, height, nrChannels;
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);	// bind texture // next set properties of textures
	if (state == 1 || state == 2) {
		data = stbi_load("./floor.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		std::cout << "This type of background was not yet implemented.\n";
		throw(100);
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load the texture.\n";
		throw(100);
	}
	stbi_image_free(data);	// release the memory of the image

	// load textrure of the walls
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);	// bind texture // next set properties of textures
	if (state == 1 || state == 2) {
		data = stbi_load("./walls.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		std::cout << "This type of background was not yet implemented.\n";
		throw(100);
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load the texture.\n";
		throw(100);
	}
	stbi_image_free(data);	// release the memory of the image

		// load textrure of the ceiling
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);	// bind texture // next set properties of textures
	if (state == 1 || state == 2) {
		data = stbi_load("./ceiling.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		std::cout << "This type of background was not yet implemented.\n";
		throw(100);
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	// copy data to GPU
	}
	else {
		std::cout << "Failed to load the texture.\n";
		throw(100);
	}
	stbi_image_free(data);	// release the memory of the image
}

void background::draw(const Shader& _SO) {
	_SO.setInt("ourTexture", 0);
	unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
	glUniform3f(colorID, 1.0f, 1.0f, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);
	glBindVertexArray(VAO[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);
	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);
	glBindVertexArray(VAO[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
