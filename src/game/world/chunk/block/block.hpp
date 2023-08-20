#pragma once

#include <tuple>
#include <glm/glm.hpp>
class Block {
public:
    enum class BlockType : std::uint8_t {
        AIR,
        STONE
    };

    using UVTexture = glm::vec2;

private:
    BlockType blockType;
public:
    Block(BlockType bt = BlockType::STONE);
    ~Block();

    BlockType getBlockType() const;

    UVTexture getUVCoords() const;
};