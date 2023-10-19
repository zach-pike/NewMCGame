#include "shader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../utils/logger/logger.hpp"
#include "../utils/getResourcePaths/getResourcePaths.hpp"

static Logger logger("Shader Loader", Logger::FGColors::MAGENTA);

GLuint loadShaders(std::string shaderResourceFolder) {
	auto paths = getShaderPaths(shaderResourceFolder);

	const std::string& vertexFilePath = paths.first;
	const std::string& fragmentFilePath = paths.second;

	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);

	if (vertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	} else {
		logger.error("Unable to open file " + vertexFilePath);

		throw std::runtime_error("Err");
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
	if (fragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	} else {
		logger.error("Could not open file " + fragmentFilePath);
		throw std::runtime_error("Err");
	}

	GLint result = GL_FALSE;
	int infoLogLength;
	
	// Compile Vertex Shader
	logger.info("Compiling Vextex shader" + vertexFilePath);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		logger.error("Error in Vertex Shader (below)\n" + vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	logger.info("Compiling Fragment shader " + fragmentFilePath);
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		logger.error("Error in Fragment Shader (below)\n" + fragmentShaderErrorMessage[0]);
	}

	// Link the program
	logger.info("Linking shaders");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		logger.error("Error linking program (below)\n" + programErrorMessage[0]);
	}
	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

