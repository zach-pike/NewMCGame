#pragma once

#include <GL/glew.h>

class World;

class ChunkBorderDebugger {
public:
    struct ChunkBorderDebuggerDrawInfo {
        GLuint vertexBuffer, colorBuffer;
        std::size_t nVerts;
    };
private:
    ChunkBorderDebuggerDrawInfo info;
public:
    ChunkBorderDebugger();
    ChunkBorderDebugger(const ChunkBorderDebugger&) = delete;
    ChunkBorderDebugger(const ChunkBorderDebugger&&) = delete;

    void draw(const World&);

    ChunkBorderDebuggerDrawInfo getDrawingInfo() const;

    ~ChunkBorderDebugger();
};