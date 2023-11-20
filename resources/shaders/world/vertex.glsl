#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in int layerId;
layout(location = 2) in int faceId;

out vec2 UV;
out float textureLayer;
out float shading;

uniform mat4 viewProjection;
uniform vec3 chunkCoord;

// Keep in mind theese are for a texture that is upside down because stbi is wierd
vec2[] UVs = vec2[6](
    vec2(0, 1), vec2(0, 0), vec2(1, 0),
    vec2(0, 1), vec2(1, 0), vec2(1, 1)
);

const float shadings[] = float[6](
    0.65, 0.8,
    0.65, 0.8,
    1.0, 0.65
);

void main(){
    gl_Position =  viewProjection * vec4(vertexPositionModelspace + (chunkCoord * 16), 1);
    UV = UVs[gl_VertexID % 6];
    shading = shadings[faceId];
    textureLayer = layerId;
}
