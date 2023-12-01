#pragma once

#include <GL/glew.h>
#include <string>

/**
 * Loads in shaders and creates a program for them
 * @param vertexFilePath Path to vertex shader
 * @param fragmentFilePath Path to fragment shader
*/
GLuint loadShaders(std::string f);