/* 
Shader class provides object for loading and using shaders in OpenGL.
The shader codes are expected to be stored under path given to constructor.
They should be written in the GLSL code.
The class methods then compiles it, initializes shaders, activates them, etc.
*/

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifndef _GLAD_
#define _GLAD_
#include <glad/glad.h>	// API for pointers in OpenGL
#endif
#ifndef _GLFW3_
#define _GLFW3_
#include <glfw3.h>		// API for context creation
#endif

#include "../logger/logger.h"

class Shader {
	// default constructor is made private in order to avid wrong usage
	Shader() { ID = 0; };
public:
	unsigned int ID;

	// constructor reads file under paths and creates shaders
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();	// for using or activating shader

	// functions for uniform setters
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;


};
#endif
