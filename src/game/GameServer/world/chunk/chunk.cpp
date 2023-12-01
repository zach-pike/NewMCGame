#include "chunk.hpp"

#include <string.h>
#include "../world.hpp"
#include <stdexcept>

Block& Chunk::getBlockReference(glm::vec3 localPos) {
    int i = (int)localPos.x + (int)localPos.z * 16 + (int)localPos.y * 16 * 16;
    return blocks.at(i);
}

Chunk::Chunk() {
    blocks.resize(16*16*16);
}
Chunk::Chunk(Chunk&& ochunk) {
    blocks = std::move(ochunk.blocks);
}
Chunk::~Chunk() {}

Block Chunk::getBlock(glm::vec3 pos) {
    return getBlockReference(pos);
}

void Chunk::setBlock(glm::vec3 pos, Block block) {
    getBlockReference(pos) = block;
    meshUpdatedNeeded = true;
}

std::vector<std::uint8_t> Chunk::serialize() const {
    const std::uint8_t* blockData = (std::uint8_t*)blocks.data();
    return std::vector<std::uint8_t>(blockData, blockData + blocks.size() * sizeof(Block));
}
void Chunk::deserialize(const std::vector<std::uint8_t>& data) {
    const Block* blockData = (Block*)data.data();
    blocks = std::vector<Block>(blockData, blockData + data.size());
}

std::size_t Chunk::serializedChunkSize() {
    return 16 * 16 * 16 * sizeof(Block);
}