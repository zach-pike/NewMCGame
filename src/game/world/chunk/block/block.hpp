#pragma once

#include <tuple>

class Block {
public:
    enum class BlockType {
        AIR,
        STONE
    };

    using UVTexture = std::tuple<int, int>;

private:
    BlockType blockType;
public:
    Block(BlockType bt = BlockType::AIR);
    ~Block();

    UVTexture getUVCoords() const;
};