#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>

#include <vector>

#include "block/block.hpp"
#include "ChunkMesh/ChunkMesh.hpp"

class World;

class Chunk {
private:
    std::vector<Block> blocks;

    ChunkMesh mesh;
    bool meshUpdatedNeeded = true;

    Block& getBlockReference(glm::vec3 pos);
public:
    Chunk();
    Chunk(Chunk&&);
    ~Chunk();

    /**
     * Returns a reference to the ChunkMesh object
    */
   const ChunkMesh& getMeshRef() const;

    /**
     * Mark the chunk to be rerendered
    */
    void markRerender();

    /**
     * Return true if the mesh in the GPU is outdated
    */
    bool pendingMeshUpdate() const;

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
     * Generates the mesh for the chunk
     * @param world The world object
     * @param chunkCoord The chunk coordinate of this chunk
    */
    void buildMesh(World& world, glm::vec3 chunkCoord);

    // For world saving
    std::vector<std::uint8_t> serialize() const;
    void deserialize(const std::vector<std::uint8_t>&);
    static std::size_t serializedChunkSize();
};