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
    mesh = std::move(ochunk.mesh);
    meshUpdatedNeeded = ochunk.meshUpdatedNeeded;
}
Chunk::~Chunk() {}

const ChunkMesh& Chunk::getMeshRef() const {
    return mesh;
}

void Chunk::markRerender() {
    meshUpdatedNeeded = true;
}
bool Chunk::pendingMeshUpdate() const {
    return meshUpdatedNeeded;
}

Block Chunk::getBlock(glm::vec3 pos) {
    return getBlockReference(pos);
}

void Chunk::setBlock(glm::vec3 pos, Block block) {
    getBlockReference(pos) = block;
    meshUpdatedNeeded = true;
}

void Chunk::buildMesh(World& world, glm::vec3 chunkCoords) {
    meshUpdatedNeeded = false;

    std::vector<glm::vec3> vertices;
    std::vector<GLint> layers;

    // Chunk generation code
    for (int y=0; y<16; y++) {
        for (int z=0; z<16; z++) {
            for (int x=0; x<16; x++) {
                auto cPos = glm::vec3(x, y, z);

                // Get the block by using its chunk coordinates
                auto block = getBlockReference(cPos);

                // if its air just do nothing
                if (block.getBlockType() == 0) continue;

                using namespace glm;

                auto addFace = [&](std::array<glm::vec3, 6> verts, Block::BlockFace face) {
                    vertices.insert(vertices.end(), verts.begin(), verts.end());
                    auto data = world.getBlockDBRef().getBlockInfoByID(block.getBlockType());

                    int layerId;
                    switch(face){
                        case Block::BlockFace::NORTH:  layerId = data.faces.north; break;
                        case Block::BlockFace::SOUTH:  layerId = data.faces.south; break;
                        case Block::BlockFace::EAST:   layerId = data.faces.east;  break;
                        case Block::BlockFace::WEST:   layerId = data.faces.west;  break;
                        case Block::BlockFace::TOP:    layerId = data.faces.top;   break;
                        case Block::BlockFace::BOTTOM: layerId = data.faces.bottom; break;
                    }

                    for (int i=0; i<6; i++) layers.push_back(layerId);
                };
                
                // +X Face Check
                const std::array<glm::vec3, 6> xPos = {
                    glm::vec3(x + 1, y, z), glm::vec3(x + 1, y + 1, z),     glm::vec3(x + 1, y + 1, z + 1),
                    glm::vec3(x + 1, y, z), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x + 1, y, z + 1),
                };
                if (x >= 15) {
                    if (chunkCoords.x + 1 >= world.chunkSizeX()
                    || world.getChunkRef(chunkCoords + vec3(1, 0, 0)).getBlock(vec3(0.f, cPos.y, cPos.z)).getBlockType() == 0)
                        addFace(xPos, Block::BlockFace::NORTH);
                } else if (getBlockReference(cPos + vec3(1, 0, 0)).getBlockType() == 0)
                    addFace(xPos, Block::BlockFace::NORTH);

                // -X Face Check
                const std::array<glm::vec3, 6> xNeg = {
                    glm::vec3(x, y, z + 1), glm::vec3(x, y + 1, z + 1), glm::vec3(x, y + 1, z),
                    glm::vec3(x, y, z + 1), glm::vec3(x, y + 1, z),     glm::vec3(x, y, z),
                };
                if (x < 1) {
                    if (chunkCoords.x - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(-1, 0, 0)).getBlock(vec3(15.f, cPos.y, cPos.z)).getBlockType() == 0)
                        addFace(xNeg, Block::BlockFace::SOUTH);
                } else if (getBlockReference(cPos + vec3(-1, 0, 0)).getBlockType() == 0)
                    addFace(xNeg, Block::BlockFace::SOUTH);

                // +Z Face Check
                const std::array<glm::vec3, 6> zPos = {
                    glm::vec3(x + 1, y, z + 1), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x, y + 1, z + 1),
                    glm::vec3(x + 1, y, z + 1), glm::vec3(x, y + 1, z + 1),     glm::vec3(x, y, z + 1),
                };
                if (z >= 15) {
                    if (chunkCoords.z + 1 >= world.chunkSizeZ()
                    || world.getChunkRef(chunkCoords + vec3(0, 0, 1)).getBlock(vec3(cPos.x, cPos.y, 0)).getBlockType() == 0)
                        addFace(zPos, Block::BlockFace::EAST);
                } else if (getBlockReference(cPos + vec3(0, 0, 1)).getBlockType() == 0)
                    addFace(zPos, Block::BlockFace::EAST);

                // -Z Face Check
                const std::array<glm::vec3, 6> zNeg = {
                    glm::vec3(x, y, z), glm::vec3(x, y + 1, z),     glm::vec3(x + 1, y + 1, z),
                    glm::vec3(x, y, z), glm::vec3(x + 1, y + 1, z), glm::vec3(x + 1, y, z),
                };
                if (z < 1) {
                    if (chunkCoords.z - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(0, 0, -1)).getBlock(vec3(cPos.x, cPos.y, 15)).getBlockType() == 0)
                        addFace(zNeg, Block::BlockFace::WEST);
                } else if (getBlockReference(cPos + vec3(0, 0, -1)).getBlockType() == 0)
                    addFace(zNeg, Block::BlockFace::WEST);

                // +Y Face Check
                const std::array<glm::vec3, 6> yPos = {
                    glm::vec3(x, y + 1, z), glm::vec3(x, y + 1, z + 1),     glm::vec3(x + 1, y + 1, z + 1),
                    glm::vec3(x, y + 1, z), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x + 1, y + 1, z),
                };
                if (y >= 15) {
                    if (chunkCoords.y + 1 >= world.chunkSizeY()
                    || world.getChunkRef(chunkCoords + vec3(0, 1, 0)).getBlock(vec3(cPos.x, 0, cPos.z)).getBlockType() == 0)
                        addFace(yPos, Block::BlockFace::TOP);
                } else if (getBlockReference(cPos + vec3(0, 1, 0)).getBlockType() == 0)
                    addFace(yPos, Block::BlockFace::TOP);

                // -Y Face Check
                const std::array<glm::vec3, 6> yNeg = {
                    glm::vec3(x, y, z), glm::vec3(x + 1, y, z),     glm::vec3(x + 1, y, z + 1),
                    glm::vec3(x, y, z), glm::vec3(x + 1, y, z + 1), glm::vec3(x, y, z + 1),
                };
                if (y < 1) {
                    if (chunkCoords.y - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(0, -1, 0)).getBlock(vec3(cPos.x, 15, cPos.z)).getBlockType() == 0)
                        addFace(yNeg, Block::BlockFace::BOTTOM);
                } else if (getBlockReference(cPos + vec3(0, -1, 0)).getBlockType() == 0)
                    addFace(yNeg, Block::BlockFace::BOTTOM);
            }
        }
    }
    
    mesh.bufferChunkData(vertices, layers);
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