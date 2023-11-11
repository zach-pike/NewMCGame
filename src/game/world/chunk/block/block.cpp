#include "block.hpp"

#include <stdexcept>

Block::Block(unsigned int _blockId, std::uint64_t _packHash):
    blockId(_blockId),
    packHash(_packHash) {}
Block::Block(std::pair<std::uint64_t, unsigned int> ident):
    packHash(ident.first),
    blockId(ident.second) {}
    
Block::~Block() {}

unsigned int Block::getBlockId() const {
    return blockId;
}
std::pair<std::uint64_t, unsigned int> Block::getIdent() const {
    return std::pair<std::uint64_t, unsigned int>{ packHash, blockId };
}