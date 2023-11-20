#include "ChunkMesh.hpp"

#include <GL/glew.h>

#include <stdexcept>
#include <exception>

ChunkMesh::ChunkMesh() {}
ChunkMesh::ChunkMesh(ChunkMesh&& ochunk) {
    vertexCount = ochunk.vertexCount;
    vertexBuffer = ochunk.vertexBuffer;
    layerBuffer = ochunk.layerBuffer;
    buffersCreated = ochunk.buffersCreated;

    ochunk.vertexCount = 0;
    ochunk.vertexBuffer = 0;
    ochunk.layerBuffer = 0;
    ochunk.buffersCreated = false;
}

void ChunkMesh::operator=(ChunkMesh&& ochunk) {
    vertexCount = ochunk.vertexCount;
    vertexBuffer = ochunk.vertexBuffer;
    layerBuffer = ochunk.layerBuffer;
    buffersCreated = ochunk.buffersCreated;

    ochunk.vertexCount = 0;
    ochunk.vertexBuffer = 0;
    ochunk.layerBuffer = 0;
    ochunk.buffersCreated = false;
}

ChunkMesh::~ChunkMesh() {
    if (buffersCreated) deleteBuffers();
}

void ChunkMesh::createBuffers() {
    if (buffersCreated) throw std::runtime_error("Buffers already created!");

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &layerBuffer);

    glGenBuffers(1, &faceIdBuffer);

    buffersCreated = true;
}

void ChunkMesh::deleteBuffers() {
    if (!buffersCreated) throw std::runtime_error("Buffers don't exist!");

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &layerBuffer);
    
    glDeleteBuffers(1, &faceIdBuffer);

    buffersCreated = false;
}

void ChunkMesh::bufferChunkData(std::span<glm::vec3> vertexInformation, std::span<GLint> layerInformation, std::span<GLint> faceIdInformation) {
    if (!buffersCreated) createBuffers();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexInformation.size() * sizeof(glm::vec3), vertexInformation.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, layerBuffer);
	glBufferData(GL_ARRAY_BUFFER, layerInformation.size() * sizeof(GLint), layerInformation.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, faceIdBuffer);
	glBufferData(GL_ARRAY_BUFFER, faceIdInformation.size() * sizeof(GLint), faceIdInformation.data(), GL_STATIC_DRAW);

    vertexCount = vertexInformation.size();
}