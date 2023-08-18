#include "block.hpp"

Block::Block(BlockType bt = BlockType::AIR):
    blockType(bt) {}

Block::~Block() {}

Block::UVTexture Block::getUVCoords() const {
    switch (blockType) {
        case BlockType::STONE: {
            return UVTexture(1, 0);
        } break;
    }
}