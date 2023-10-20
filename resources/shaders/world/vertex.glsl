#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in int layerId;

out vec2 UV;
out float textureLayer;

uniform mat4 viewProjection;
uniform vec3 chunkCoord;

vec2[] UVs = vec2[6](
    vec2(0, 1), vec2(0, 0), vec2(1, 0),
    vec2(0, 1),  vec2(1, 0), vec2(1, 1)
);

void main(){
    gl_Position =  viewProjection * vec4(vertexPositionModelspace + (chunkCoord * 16), 1);
    UV = UVs[gl_VertexID % 6];
    textureLayer = layerId;
}
