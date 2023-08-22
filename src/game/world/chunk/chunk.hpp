#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>

#include "block/block.hpp"

#include "../../types/types.hpp"

#include <vector>

class World;

class Chunk {
public:
    struct BufferInfo {
        GLuint vertexBuffer, uvBuffer;
    };

private:
    bool _hasBeenMoved = false;

    std::vector<Block> blocks;

    BufferInfo buffers;
    std::size_t nVertices;

    bool meshUpdatedNeeded = true;

    void blockDrawer(
        std::array<Vertex, 36>& vtx_data,
        std::array<UV, 36>& uv_data,
        std::size_t& index,
        glm::vec3 cPos, // Chunk position each axis that follows 0 <= x < 16
        glm::vec3 chunkCoords,
        World& world
    ) const;

    Block& getBlockReference(glm::vec3 pos);
public:
    Chunk();
    Chunk(Chunk&&);
    ~Chunk();

    bool pendingMeshUpdate() const;

    std::size_t getNVertices() const;

    Block getBlock(glm::vec3 localPos);
    void setBlock(glm::vec3 localPos, Block block);

    BufferInfo getBufferInfo() const;

    // Mark the chunk to be rerendered
    void markRerender();

    void update(glm::vec3 chunkCoord, World& world);
};