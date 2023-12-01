#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <span>

class ChunkMesh {
private:
    std::size_t vertexCount = 0;
    GLuint vertexBuffer, layerBuffer;
    bool buffersCreated = false;
public:
    ChunkMesh();
    ChunkMesh(ChunkMesh&&);
    void operator=(ChunkMesh&&);
    ~ChunkMesh();

    void createBuffers();
    void deleteBuffers();
    void bufferChunkData(std::span<glm::vec3> vertexInformation, std::span<GLint> layerInformation);

    inline std::size_t getVertexCount() const { return vertexCount; }
    inline bool buffersReady() const { return buffersCreated; }
    inline GLuint getVertexBuffer() const { return vertexBuffer; }
    inline GLuint getLayerBuffer() const { return layerBuffer; }
};