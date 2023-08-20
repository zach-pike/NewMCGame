#pragma once

#include <tuple>
#include <glm/glm.hpp>
class Block {
public:
    enum class BlockType : std::uint8_t {
        AIR,
        DIRT,
        GRASS,
        STONE
    };

    using UVTexture = glm::vec2;

private:
    BlockType blockType;
public:
    Block(BlockType bt = BlockType::AIR);
    ~Block();

    enum class BlockFace {
        NORTH, // +X
        SOUTH, // -X
        EAST,  // +Z
        WEST,  // -Z
        TOP,   // +Y
        BOTTOM // -Y
    };

    BlockType getBlockType() const;

    UVTexture getUVCoords(BlockFace) const;
};