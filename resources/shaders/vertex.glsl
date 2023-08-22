#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in vec2 uv_coords;

out vec2 UV;

uniform mat4 viewProjection;
uniform vec3 chunkCoord;

void main(){
    gl_Position =  viewProjection * vec4(vertexPositionModelspace + (chunkCoord * 16), 1);
    UV = uv_coords;
}
