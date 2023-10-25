#pragma once

#include <tuple>
#include <glm/glm.hpp>

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
    
    using BlockType = unsigned int;

private:
    BlockType blockType;
public:
    Block(BlockType bt = 0);
    ~Block();

    /**
     * Get the block type
    */
    BlockType getBlockType() const;
};