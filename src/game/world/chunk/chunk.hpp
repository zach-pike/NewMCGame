#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>

#include "block/block.hpp"

#include "../../types/types.hpp"

#include <vector>

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
        glm::vec3 gPos, // Global coordinates of thr block (WorldPos)
        glm::vec3 cPos, // Chunk position each axis that follows 0 <= x < 16
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

    void update(glm::vec3 chunkPos, World& world);
};