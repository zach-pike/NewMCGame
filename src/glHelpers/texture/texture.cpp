#include "texture.hpp"

#include <stdio.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

#include "glHelpers/utils/getResourcePaths/getResourcePaths.hpp"
#include "glHelpers/utils/logger/logger.hpp"

static Logger logger("Texture Loader", Logger::FGColors::CYAN);

GLuint loadImageTexture(std::string s) {
	auto path = getTexturePath(s);

	int width, height, channels;
	unsigned char* img = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);

	if(img == NULL) {
		logger.error("Could not open file " + path);
 		exit(1);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);

	logger.info("Created texture [ID:" + std::to_string(textureID) + "]" + " Width: " + std::to_string(width) + " Height: " + std::to_string(height));
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	// Use nearest neighbor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	stbi_image_free(img);

	return textureID;
}