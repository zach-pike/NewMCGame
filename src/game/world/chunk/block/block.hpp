#pragma once

#include <tuple>
#include <glm/glm.hpp>
#include "types/types.hpp"

class Block {
public:
    enum class BlockFace {
        NORTH, // +X
        SOUTH, // -X
        EAST,  // +Z
        WEST,  // -Z
        TOP,   // +Y
        BOTTOM // -Y
    };

private:
    using BlockType = unsigned int;
    BlockType blockType;
public:
    Block(BlockType bt = 0);
    ~Block();

    /**
     * Get the block type
    */
    BlockType getBlockType() const;

    /**
     * Get the UV coordinates for a specific side of the block
     * @param face Block face
     * @deprecated using array textures now
    */
    // UV getUVCoords(BlockFace face) const;
};