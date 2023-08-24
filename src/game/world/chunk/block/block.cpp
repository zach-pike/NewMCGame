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
        case BlockType::STONE: return UV(1, 0);
        case BlockType::DIRT: return UV(2, 0);

        case BlockType::GRASS: {
            switch(f) {
                case BlockFace::NORTH:
                case BlockFace::SOUTH:
                case BlockFace::EAST:
                case BlockFace::WEST:
                    return UV(3, 0);

                case BlockFace::BOTTOM: return UV(2, 0);
                case BlockFace::TOP: return UV(0, 0);
            }
        };

        case BlockType::OAK_WOOD: return UV(4, 0);
        case BlockType::DOUBLE_SLAB: {
            switch(f) {
                case BlockFace::NORTH:
                case BlockFace::SOUTH:
                case BlockFace::EAST:
                case BlockFace::WEST:
                    return UV(5, 0);
                case BlockFace::TOP:
                case BlockFace::BOTTOM:
                    return UV(6, 0);
            }
        };
        case BlockType::WHOLE_SLAB: return UV(6, 0);
        case BlockType::BRICKS: return UV(7, 0);
        case BlockType::TNT: {
            switch(f) {
                case BlockFace::NORTH:
                case BlockFace::SOUTH:
                case BlockFace::EAST:
                case BlockFace::WEST:
                    return UV(8, 0);
                case BlockFace::TOP:
                    return UV(9, 0);
                case BlockFace::BOTTOM:
                    return UV(10, 0);
            }
        };
        case BlockType::COBBLESTONE: return UV(0, 1);
        case BlockType::BEDROCK: return UV(1, 1);
        case BlockType::SAND: return UV(2, 1);
        case BlockType::GRAVEL: return UV(3, 1);
    }

    throw std::runtime_error("Invalid block type or face");
}