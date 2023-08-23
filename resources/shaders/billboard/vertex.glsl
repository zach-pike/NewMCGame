#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in vec2 textureCoords;

uniform vec3 cameraPosition;
uniform vec3 cameraRightWorldspace;
uniform vec3 cameraUpWorldspace;
uniform mat4 cameraViewProjection;

uniform vec3 modelPosition;
uniform vec2 billboardScale;
out vec2 UV;

void main(){
    vec3 vertexPositionWorldspace =
        modelPosition
        + cameraRightWorldspace * vertexPositionModelspace.x * billboardScale.x
        + cameraUpWorldspace * vertexPositionModelspace.y * billboardScale.y;

    gl_Position = cameraViewProjection * vec4(vertexPositionWorldspace, 1);

    UV = textureCoords;
}