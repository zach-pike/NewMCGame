#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in vec3 color_in;

out vec3 fragColor;

uniform mat4 viewProjection;

void main(){
    gl_Position =  viewProjection * vec4(vertexPositionModelspace, 1);
    fragColor = color_in;
}
