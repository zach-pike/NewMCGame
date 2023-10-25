#include "block.hpp"

#include <stdexcept>

Block::Block(BlockType bt):
    blockType(bt) {}

Block::~Block() {}

Block::BlockType Block::getBlockType() const {
    return blockType;
}