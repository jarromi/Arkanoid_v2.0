/* Now we create a shader class that will read the GLSL code, compile it, initialize shaders, activate shaders, ...*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>	// API for pointers in OpenGL
#include <glfw3.h>		// API for context creation

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);	// constructor reads file under paths and creates shaders

	void use();	// for using or activating shader

	// functions for uniform setters
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;


};
/*
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::fstream VSF, FSF;
	char line[256];
	std::string sourceCode;

	//open, read file with code for vertex shader, create and compile, close file
	try
	{
		VSF.open(vertexPath, std::fstream::in);
		while (VSF.getline(line, 256)) {
			sourceCode += line;
			sourceCode += "\n";
		}
		VSF.close();
	}
	catch(std::fstream::failure e){
		std::cout << "ERROR while reading vertex shader file: " << std::endl;
	}
	char* charSourceCode;
	charSourceCode = new char [sourceCode.size() + 1];
	strcpy_s(charSourceCode, sourceCode.size() + 1,sourceCode.c_str());

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &charSourceCode, NULL);
	glCompileShader(vertexShader);
	delete[] charSourceCode;

	// now optional to check compilation status
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: vertex shader compilation failed: " << infoLog << std::endl;
	}

	//open, read file with code for fragment shader, create and compile, close file
	sourceCode = "";
	try {
		FSF.open(fragmentPath, std::fstream::in);
		while (FSF.getline(line, 256)) {
			sourceCode += line;
			sourceCode += "\n";
		}
		FSF.close();
	}
	catch (std::fstream::failure e) {
		std::cout << "ERROR while reading fragment shader file: " << std::endl;
	}
	charSourceCode = new char[sourceCode.size() + 1];
	strcpy_s(charSourceCode, sourceCode.size() + 1, sourceCode.c_str());

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &charSourceCode, NULL);
	glCompileShader(fragmentShader);
	delete[] charSourceCode;

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: fragment shader compilation failed: " << infoLog << std::endl;
	}

	ID = glCreateProgram();	// create program and assign id

	glAttachShader(ID, vertexShader);	//attach vertex shader
	glAttachShader(ID, fragmentShader);	//attach fragment shader
			// we only attach those which we modified/defined
	glLinkProgram(ID);	// link all of the shaders

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR: shader program linking failed: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);	// the shader sourcecode objects we defined are no longer needed, linking was already done
	glDeleteShader(fragmentShader);

}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}*/
#endif
