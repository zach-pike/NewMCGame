#version 330 core

// Ouput data
out vec3 color;

in vec2 UV;

void main() {
	// Output color = color of the texture at the specified UV
	color = vec3(1, 0, 0);
}