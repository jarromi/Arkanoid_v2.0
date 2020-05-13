#include "shader.h"

/*
The constructor with paths to the vertex and fragment shaders.
It reads the shader programs from files, compiles the vertex programs,
initializes shaders and activates them.
*/
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
	catch (std::fstream::failure e) {
		logger::log("ERROR while reading vertex shader file.\n");
	}
	char* charSourceCode;
	charSourceCode = new char[sourceCode.size() + 1];
	strcpy_s(charSourceCode, sourceCode.size() + 1, sourceCode.c_str());

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
		std::stringstream ss;
		ss << "ERROR: vertex shader compilation failed: " << infoLog << std::endl;
		logger::log(ss);
	}

	// Do the same as above for the fragment shader
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
		logger::log("ERROR while reading fragment shader file\n.");
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
		std::stringstream ss;
		ss << "ERROR: fragment shader compilation failed: " << infoLog << std::endl;
		logger::log(ss);
	}

	// create program and assign id
	ID = glCreateProgram();	

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// Link shaders
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::stringstream ss;
		ss << "ERROR: shader program linking failed: " << infoLog << std::endl;
		logger::log(ss);
	}

	glDeleteShader(vertexShader);	// the shader sourcecode objects we defined are no longer needed, linking was already done
	glDeleteShader(fragmentShader);

}

// Method for using shaders in progaram
void Shader::use() {
	glUseProgram(ID);
}

// Additional methods for seting unifroms in shaders
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
}