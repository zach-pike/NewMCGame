#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 textureCoords;

out vec2 UV;

// uniform vec2 scale;
uniform float aspect;

void main(){
    gl_Position = vec4(vertexPosition * vec2(1, aspect), 0, 1);
    UV = textureCoords;
}