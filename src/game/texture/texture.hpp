#pragma once
#include <GL/glew.h>
#include <string>

/**
 * Load a image as a texture
 * Supports any format that stbi_image.h supports
 * @param s Path to the texture
*/
GLuint loadImageTexture(std::string s);