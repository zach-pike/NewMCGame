#include "chunk.hpp"

#include <string.h>
#include "../world.hpp"
#include <stdexcept>

void Chunk::blockDrawer(
    std::array<Vertex, 36>& vtx_data,
    std::array<UV, 36>& uv_data,
    std::size_t& index,
    glm::vec3 cPos,        // XYZ position within a chunk
    glm::vec3 chunkCoords, // Chunk coordinates
    World& world
) const {
    auto getBlockInChunk = [&](glm::vec3 b_pos) {
        int i = b_pos.x + b_pos.z * 16 + b_pos.y * 16 * 16;
        return blocks.at(i);
    };

    auto vec3totuple = [](glm::vec3 pos) {
        return std::tuple<int, int, int>(pos.x, pos.y, pos.z);
    };

    // Get global block coordinates
    auto x = cPos.x;
    auto y = cPos.y;
    auto z = cPos.z;

    // Get the block by using its chunk coordinates
    auto block = getBlockInChunk(cPos);

    // if its air just do nothing
    if (block.getBlockType() == Block::BlockType::AIR) return;

    const int faceSideLen = 6;
    
    const std::array<Vertex, 6> zNeg = {
        // -Z Face
        Vertex(x, y, z), Vertex(x, y + 1, z),     Vertex(x + 1, y + 1, z),
        Vertex(x, y, z), Vertex(x + 1, y + 1, z), Vertex(x + 1, y, z),
    };
    const std::array<Vertex, 6> zPos = {
        // +Z Face
        Vertex(x + 1, y + 1, z + 1), Vertex(x, y + 1, z + 1),     Vertex(x, y, z + 1),
        Vertex(x + 1, y, z + 1),     Vertex(x + 1, y + 1, z + 1), Vertex(x, y, z + 1),
    };
    const std::array<Vertex, 6> xPos = {
        Vertex(x + 1, y, z), Vertex(x + 1, y + 1, z),     Vertex(x + 1, y + 1, z + 1),
        Vertex(x + 1, y, z), Vertex(x + 1, y + 1, z + 1), Vertex(x + 1, y, z + 1),
    };
    const std::array<Vertex, 6> xNeg = {
        Vertex(x, y + 1, z + 1), Vertex(x, y + 1, z),     Vertex(x, y, z),
        Vertex(x, y, z + 1), Vertex(x, y + 1, z + 1), Vertex(x, y, z),
    };
    const std::array<Vertex, 6> yNeg = {
        Vertex(x, y, z), Vertex(x + 1, y, z),     Vertex(x + 1, y, z + 1),
        Vertex(x, y, z), Vertex(x + 1, y, z + 1), Vertex(x, y, z + 1),
    };
    const std::array<Vertex, 6> yPos = {
        Vertex(x + 1, y + 1, z + 1), Vertex(x + 1, y + 1, z),     Vertex(x, y + 1, z),
        Vertex(x, y + 1, z + 1),     Vertex(x + 1, y + 1, z + 1), Vertex(x, y + 1, z),
    };

    using namespace glm;

    auto addFace = [&vtx_data, &uv_data, &index, block](std::array<Vertex, 6> verts, bool uvsToUse, Block::BlockFace face) {
        std::copy(verts.begin(), verts.end(), vtx_data.begin() + index);
        auto blockBaseUV = block.getUVCoords(face);

        const std::array<UV, 6> oddUVs = {
            blockBaseUV + UV(0, 1), blockBaseUV,            blockBaseUV + UV(1, 0),
            blockBaseUV + UV(0, 1), blockBaseUV + UV(1, 0), blockBaseUV + UV(1, 1),
        };
        const std::array<UV, 6> evenUVs = {
            blockBaseUV + UV(1, 0), blockBaseUV,            blockBaseUV + UV(0, 1),
            blockBaseUV + UV(1, 1), blockBaseUV + UV(1, 0), blockBaseUV + UV(0, 1),
        };
        if (uvsToUse) {
            std::copy(evenUVs.begin(), evenUVs.end(), uv_data.begin() + index);
        } else {
            std::copy(oddUVs.begin(), oddUVs.end(), uv_data.begin() + index);
        }
        
        index += faceSideLen;
    };
    // +X Face Check
    if (cPos.x >= 15) {
        if (chunkCoords.x + 1 >= world.chunkSizeX()
         || world.getChunkRef(chunkCoords + vec3(1, 0, 0)).getBlock(vec3(0.f, cPos.y, cPos.z)).getBlockType() == Block::BlockType::AIR)
            addFace(xPos, false, Block::BlockFace::NORTH);
    } else if (getBlockInChunk(cPos + vec3(1, 0, 0)).getBlockType() == Block::BlockType::AIR)
        addFace(xPos, false, Block::BlockFace::NORTH);

    // -X Face Check
    if (cPos.x < 1) {
        if (chunkCoords.x - 1 < 0
         || world.getChunkRef(chunkCoords + vec3(-1, 0, 0)).getBlock(vec3(15.f, cPos.y, cPos.z)).getBlockType() == Block::BlockType::AIR)
            addFace(xNeg, true, Block::BlockFace::SOUTH);
    } else if (getBlockInChunk(cPos + vec3(-1, 0, 0)).getBlockType() == Block::BlockType::AIR)
        addFace(xNeg, true, Block::BlockFace::SOUTH);

    // +Z Face Check
    if (cPos.z >= 15) {
        // Next block over
        if (chunkCoords.z + 1 >= world.chunkSizeZ()
         || world.getChunkRef(chunkCoords + vec3(0, 0, 1)).getBlock(vec3(cPos.x, cPos.y, 0)).getBlockType() == Block::BlockType::AIR)
            addFace(zPos, true, Block::BlockFace::EAST);
    } else if (getBlockInChunk(cPos + vec3(0, 0, 1)).getBlockType() == Block::BlockType::AIR)
        addFace(zPos, true, Block::BlockFace::EAST);

    // -Z Face Check
    if (cPos.z < 1) {
        // Next block over
        if (chunkCoords.z - 1 < 0
         || world.getChunkRef(chunkCoords + vec3(0, 0, -1)).getBlock(vec3(cPos.x, cPos.y, 15)).getBlockType() == Block::BlockType::AIR)
            addFace(zNeg, false, Block::BlockFace::WEST);
    } else if (getBlockInChunk(cPos + vec3(0, 0, -1)).getBlockType() == Block::BlockType::AIR)
        addFace(zNeg, false, Block::BlockFace::WEST);

    // +Y Face Check
    if (cPos.y >= 15) {
        if (chunkCoords.y + 1 >= world.chunkSizeY()
         || world.getChunkRef(chunkCoords + vec3(0, 1, 0)).getBlock(vec3(cPos.x, 0, cPos.z)).getBlockType() == Block::BlockType::AIR)
            addFace(yPos, true, Block::BlockFace::TOP);
    } else if (getBlockInChunk(cPos + vec3(0, 1, 0)).getBlockType() == Block::BlockType::AIR)
        addFace(yPos, true, Block::BlockFace::TOP);

    // -Y Face Check
    if (cPos.y < 1) {
        if (chunkCoords.y - 1 < 0
         || world.getChunkRef(chunkCoords + vec3(0, -1, 0)).getBlock(vec3(cPos.x, 15, cPos.z)).getBlockType() == Block::BlockType::AIR)
            addFace(yNeg, false, Block::BlockFace::BOTTOM);
    } else if (getBlockInChunk(cPos + vec3(0, -1, 0)).getBlockType() == Block::BlockType::AIR)
        addFace(yNeg, false, Block::BlockFace::BOTTOM);
}

Block& Chunk::getBlockReference(glm::vec3 localPos) {
    int i = localPos.x + localPos.z * 16 + localPos.y * 16 * 16;

    return blocks.at(i);
}

Chunk::Chunk() {
    blocks.resize(16*16*16);

    glGenBuffers(1, &buffers.vertexBuffer);
    glGenBuffers(1, &buffers.uvBuffer);
}

Chunk::Chunk(Chunk&& ochunk) {
    blocks = ochunk.blocks;
    buffers = ochunk.buffers;
    nVertices = ochunk.nVertices;
    meshUpdatedNeeded = ochunk.meshUpdatedNeeded;

    ochunk._hasBeenMoved = true;
}

Chunk::~Chunk() {
    if (!_hasBeenMoved) {
        glDeleteBuffers(1, &buffers.vertexBuffer);
        glDeleteBuffers(1, &buffers.uvBuffer);
    }
}

void Chunk::markRerender() {
    meshUpdatedNeeded = true;
}

bool Chunk::pendingMeshUpdate() const {
    return meshUpdatedNeeded;
}

std::size_t Chunk::getNVertices() const {
    return nVertices;
}

Block Chunk::getBlock(glm::vec3 pos) {
    return getBlockReference(pos);
}

void Chunk::setBlock(glm::vec3 pos, Block block) {
    getBlockReference(pos) = block;
    meshUpdatedNeeded = true;
}

Chunk::BufferInfo Chunk::getBufferInfo() const {
    return buffers;
}

void Chunk::generateMesh(glm::vec3 chunkCoords, World& world) {
    meshUpdatedNeeded = false;

    std::vector<Vertex> vertices;
    std::vector<UV> uvs;

    // Chunk generation code
    for (int y=0; y<16; y++) {
        for (int z=0; z<16; z++) {
            for (int x=0; x<16; x++) {
                std::array<Vertex, 36> vtx;
                std::array<UV, 36> uvx;

                std::size_t index = 0;

                blockDrawer(
                    vtx,   // Vertex buffer
                    uvx,   // UV buffer
                    index, // Buffer offset
                    glm::vec3(x, y, z),  // Chunk x, y, z
                    chunkCoords,         // Chunk coordinate (chunk origin)
                    world
                );

                auto vtx_begin = vtx.begin();
                auto uvx_begin = uvx.begin();

                vertices.insert(vertices.end(), vtx_begin, vtx_begin + index);
                uvs.insert(uvs.end(), uvx_begin, uvx_begin + index);
            }
        }
    }

    nVertices = vertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(UV), uvs.data(), GL_STATIC_DRAW);
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