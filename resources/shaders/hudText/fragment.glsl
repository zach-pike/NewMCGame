#version 330 core

// Ouput data
out vec4 color;

in vec2 UV;

uniform sampler2D myTexture;

void main() {
	vec2 scaledUV = UV*0.0625;
	color = texture(myTexture, vec2(scaledUV.x, scaledUV.y));
}