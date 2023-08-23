#include "texture.hpp"

#include <stdio.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

GLuint loadImageTexture(std::string s) {
	int width, height, channels;
	unsigned char* img = stbi_load(s.c_str(), &width, &height, nullptr, STBI_rgb_alpha);

	if(img == NULL) {
		printf("Error in loading the image\n");
 		exit(1);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	// Nearest neighbor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	stbi_image_free(img);

	return textureID;
}