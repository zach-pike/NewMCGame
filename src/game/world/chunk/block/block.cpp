#include "block.hpp"

#include <stdexcept>

Block::Block(BlockType bt):
    blockType(bt) {}

Block::~Block() {}

Block::UVTexture Block::getUVCoords(BlockFace f) const {
    switch (blockType) {
        case BlockType::STONE: {
            return UVTexture(1, 0);
        } break;
        case BlockType::DIRT: {
            return UVTexture(2, 0);
        } break;
        case BlockType::GRASS: {
            switch(f) {
                case BlockFace::NORTH:
                case BlockFace::SOUTH:
                case BlockFace::EAST:
                case BlockFace::WEST: {
                    return UVTexture(3, 0);
                } break;
                case BlockFace::BOTTOM: {
                    return UVTexture(2, 0);
                } break;
                case BlockFace::TOP: {
                    return UVTexture(0, 0);
                } break;
            }
        };
    }

    throw std::runtime_error("Invalid block type or face");
}

Block::BlockType Block::getBlockType() const {
    return blockType;
}