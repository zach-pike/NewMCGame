#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureAtlas;

void main() {
	// Output color = color of the texture at the specified UV
	color = texture(textureAtlas, UV * 0.0625);
}