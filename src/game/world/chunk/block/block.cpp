#include "block.hpp"

#include <stdexcept>

Block::Block(BlockType bt):
    blockType(bt) {}

Block::~Block() {}

Block::BlockType Block::getBlockType() const {
    return blockType;
}

UV Block::getUVCoords(BlockFace f) const {
    switch (blockType) {
        case BlockType::STONE: {
            return UV(1, 0);
        } break;
        case BlockType::DIRT: {
            return UV(2, 0);
        } break;
        case BlockType::GRASS: {
            switch(f) {
                case BlockFace::NORTH:
                case BlockFace::SOUTH:
                case BlockFace::EAST:
                case BlockFace::WEST: {
                    return UV(3, 0);
                } break;
                case BlockFace::BOTTOM: {
                    return UV(2, 0);
                } break;
                case BlockFace::TOP: {
                    return UV(0, 0);
                } break;
            }
        };
    }

    throw std::runtime_error("Invalid block type or face");
}