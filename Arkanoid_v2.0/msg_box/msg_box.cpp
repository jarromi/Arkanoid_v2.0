#include "msg_box.h"

// -------------------------------------------------
// Initialization of static variables static variables

	// Since the model is fairly simple we define here all of its vertices
const float msg_box::vertices[] = {
		-0.66f, -0.1f,  0.2f, 0.0f, 1.0f,
		 0.66f, -0.1f,  0.2f, 1.0f, 1.0f,
		 0.66f,  0.1f,  0.2f, 1.0f, 0.0f,
		-0.66f,  0.1f,  0.2f, 0.0f, 0.0f
};

	// We draw the object using vertices combined into triangles.
	// Indices specify which indices to combine into which triangle.
const unsigned int msg_box::indices[] = {
		0, 1, 3,
		1, 2, 3
};

	// Textures and vertex array objects (VAO), element buffer objects (EBO)
	// and vertex buffer object (VBO) are shared between all of the objects of
	// type "msg_box".
unsigned int msg_box::TextureID = 0;
unsigned int msg_box::VAO = 0;
unsigned int msg_box::EBO = 0;
unsigned int msg_box::VBO = 0;

	// Count keeps count of how many objects have been created. Error if count>1.
unsigned int msg_box::count = 0;

//------------------------------------------------------------------------------
// Constructors and destructors
	// Default constructor
	// Loads level 1 background graphics 
msg_box::msg_box() {
	// keep count of number of bricks
	++count;

	// set the position and state
	message_on = true;
	state = 1;
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));

	// generate OpenGL texture objects 
	glGenTextures(1, &TextureID);
	// load textrure of the floor
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data;
	int width, height, nrChannels;
	data = stbi_load("./msg_box/PressSPACE.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the message texture.\n");
		throw "BAD_MSG_BOX";
	}
	stbi_image_free(data);	// release the memory of the image

	// set VAO, EBO, VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

//default destructor
msg_box::~msg_box() {
	--count;
	if (count == 0) {
		glDeleteTextures(1, &TextureID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
}

// ---------------------------------------------------------------------------------
// Ohter methods
	// Set state changes the background
	// For now only one background is available, but in future more models might be needed
void msg_box::set_state(const unsigned int& _state) {
	state = _state;
	message_on = true;

	unsigned char* data;
	int width, height, nrChannels;
	glBindTexture(GL_TEXTURE_2D, TextureID);	// bind texture // next set properties of textures
	if (state == 1) {
		data = stbi_load("./msg_box/PressSPACE.png", &width, &height, &nrChannels, 0);	// load texture
	}
	else {
		logger::log("This type of message was not yet implemented.\n");
		throw "BAD_MSG_BOX";
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		logger::log("Failed to load the message texture.\n");
		throw "BAD_MSG_BOX";
	}
	stbi_image_free(data);	// release the memory of the image

}

	// This method instructs OpenGL to draw the background
void msg_box::draw(const Shader& _SO) {
	if (message_on) {
		_SO.setInt("ourTexture", 0);
		unsigned int modelID = glGetUniformLocation(_SO.ID, "model");
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		unsigned int colorID = glGetUniformLocation(_SO.ID, "ourColor");
		glUniform3f(colorID, 1.0f, 1.0f, 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void msg_box::set_message_on() {
	message_on = true;
}

void msg_box::set_message_off() {
	message_on = false;
}
