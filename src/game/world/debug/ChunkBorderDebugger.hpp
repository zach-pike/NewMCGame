#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class World;

class ChunkBorderDebugger {
public:
    struct ChunkBorderDebuggerBuffers {
        GLuint vertexBuffer, colorBuffer;
    };
private:
    ChunkBorderDebuggerBuffers buffers;
    std::size_t nVerts;
    GLuint debugShader;
    GLuint viewProjectionID;

    void drawLines(int, int, int);
public:
    ChunkBorderDebugger(const World&);
    ChunkBorderDebugger(const ChunkBorderDebugger&) = delete;
    ChunkBorderDebugger(const ChunkBorderDebugger&&) = delete;

    void draw(const glm::mat4& viewProjection);

    ~ChunkBorderDebugger();
};