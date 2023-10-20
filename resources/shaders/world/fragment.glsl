#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in float textureLayer;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2DArray textureAtlas;

void main() {
	color = texture(textureAtlas, vec3(UV, textureLayer));
}