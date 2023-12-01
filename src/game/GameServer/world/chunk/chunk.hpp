#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "block/block.hpp"

class World;

class Chunk {
private:
    std::vector<Block> blocks;

    bool meshUpdatedNeeded = true;

    Block& getBlockReference(glm::vec3 pos);
public:
    Chunk();
    Chunk(Chunk&&);
    ~Chunk();

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

    // For world saving && multiplayer
    std::vector<std::uint8_t> serialize() const;
    void deserialize(const std::vector<std::uint8_t>&);
    static std::size_t serializedChunkSize();
};