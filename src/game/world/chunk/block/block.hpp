#pragma once

#include <tuple>
#include <glm/glm.hpp>
#include "types/types.hpp"

class Block {
public:
    enum class BlockType : std::uint8_t {
        AIR,
        DIRT,
        GRASS,
        STONE
    };

    enum class BlockFace {
        NORTH, // +X
        SOUTH, // -X
        EAST,  // +Z
        WEST,  // -Z
        TOP,   // +Y
        BOTTOM // -Y
    };

private:
    BlockType blockType;
public:
    Block(BlockType bt = BlockType::AIR);
    ~Block();

    /**
     * Get the block type
    */
    BlockType getBlockType() const;

    /**
     * Get the UV coordinates for a specific side of the block
     * @param face Block face
    */
    UV getUVCoords(BlockFace face) const;
};