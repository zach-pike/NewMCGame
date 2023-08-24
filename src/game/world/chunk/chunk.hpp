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

    /**
     * Mark the chunk to be rerendered
    */
    void markRerender();

    /**
     * Return true if the mesh in the GPU is outdated
    */
    bool pendingMeshUpdate() const;

    /**
     * Returns the number of vertices in the buffer
    */
    std::size_t getNVertices() const;

    /**
     * Get a block inside of the chunk
     * @param localPos Position inside of the chunk
    */
    Block getBlock(glm::vec3 localPos);

    /**
     * Set a block inside of the chunk
     * @param localPos The position
     * @param block Block to set
    */
    void setBlock(glm::vec3 localPos, Block block);

    /**
     * Get the buffer information for the chunk mesh
    */
    BufferInfo getBufferInfo() const;

    /**
     * Generates the mesh for the chunk
     * @param chunkCoord The chunk coordinate of this chunk
     * @param world The world object
    */
    void generateMesh(glm::vec3 chunkCoord, World& world);

    std::vector<std::uint8_t> serialize() const;
    void deserialize(const std::vector<std::uint8_t>&);

    static std::size_t serializedChunkSize();
};