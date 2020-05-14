#include "background.h"

// -------------------------------------------------
// Initialization of static variables static variables

	// Since the model of a background is fairly simple we define here all of its vertices
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

	// We draw the object using vertices combined into triangles.
	// Indices specify which indices to combine into which triangle.
	// There are three sets of indices, because different parts of the beckground will have different textures.
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

	// Textures and vertex array objects (VAO), element buffer objects (EBO)
	// and vertex buffer object (VBO) are shared between all of the objects of
	// type "background".
	// Actually, only one object of type background is ever meant to be declared.
	// Here are ids of these objects
unsigned int background::TextureID[] = {0,0,0};
unsigned int background::VAO[] = { 0,0,0 };
unsigned int background::EBO[] = { 0,0,0 };
unsigned int background::VBO = 0;

	// Count keeps count of how many objects have been created. Error if count>1.
unsigned int background::count = 0;

//------------------------------------------------------------------------------
// Constructors and destructors
	// Default constructor
	// Loads level 1 background graphics 
background::background() {
	// keep count of number of bricks
	++count;
	if (count > 1) {
		logger::log("Only one object of type 'background' was ever ment to be created.\n");
		throw "BAD_BACKGROUND";
	}

	// set the position and state
	state = 1;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));

	// generate OpenGL texture objects 
	glGenTextures(3, TextureID);
	// load textrure of the floor
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data;
	int width, height, nrChannels;
	data = stbi_load("./background/floor.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	
	}
	else {
		logger::log("Failed to load the floor texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image

	// load textrure of the walls
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./background/walls.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the walls texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image

	// load textrure of the ceiling
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./background/ceiling.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the ceiling texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image

	// set VAO, EBO, VBO
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

}

//default destructor
background::~background() {
	--count;
	if (count == 0) {
		glDeleteTextures(3, TextureID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(3, EBO);
		glDeleteVertexArrays(3, VAO);
	}
}

// ---------------------------------------------------------------------------------
// Ohter methods
	// Set state changes the background
	// For now only one background is available, but in future more models might be needed
void background::set_state(const unsigned int& _state) {
	state = _state;

	unsigned char* data;
	int width, height, nrChannels;
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);	// bind texture // next set properties of textures
	if (state == 1) {
		data = stbi_load("./background/floor.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		logger::log("This type of background was not yet implemented.\n");
		throw "BAD_BACKGROUND";
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the floor texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image

	// load textrure of the walls
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);	// bind texture // next set properties of textures
	if (state == 1) {
		data = stbi_load("./background/walls.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		logger::log("This type of background was not yet implemented.\n");
		throw "BAD_BACKGROUND";
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the walls texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image

		// load textrure of the ceiling
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);	// bind texture // next set properties of textures
	if (state == 1) {
		data = stbi_load("./background/ceiling.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		logger::log("This type of background was not yet implemented.\n");
		throw "BAD_BACKGROUND";
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the ceiling texture.\n");
		throw "BAD_BACKGROUND";
	}
	stbi_image_free(data);	// release the memory of the image
}

	// This method instructs OpenGL to draw the background
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
