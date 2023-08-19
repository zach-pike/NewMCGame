#include "chunk.hpp"

#include <string.h>

Chunk::Chunk() {}
Chunk::~Chunk() {}

void Chunk::blockDrawer(std::array<Vertex, 36>& vtx_data, std::array<UV, 36>& uv_data, std::size_t& index, glm::vec3 gPos, glm::vec3 cPos) const {
    auto getBlockInChunk = [&](glm::vec3 b_pos) {
        return blocks.at(b_pos.x + b_pos.z * 16 + b_pos.y * 16 * 16);
    };

    // Get global block coordinates
    auto x = gPos.x;
    auto y = gPos.y;
    auto z = gPos.z;

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

    auto blockBaseUV = block.getUVCoords();

    const std::array<UV, 6> oddUVs = {
        blockBaseUV + UV(0, 1), blockBaseUV,            blockBaseUV + UV(1, 0),
        blockBaseUV + UV(0, 1), blockBaseUV + UV(1, 0), blockBaseUV + UV(1, 1),
    };

    const std::array<UV, 6> evenUVs = {
        blockBaseUV + UV(1, 0), blockBaseUV,            blockBaseUV + UV(0, 1),
        blockBaseUV + UV(1, 1), blockBaseUV + UV(1, 0), blockBaseUV + UV(0, 1),
    };

    using namespace glm;

    // +X Face Check
    if (cPos.x >= 15 || getBlockInChunk(cPos + vec3(1, 0, 0)).getBlockType() == Block::BlockType::AIR) {
        std::copy(xPos.begin(), xPos.end(), vtx_data.begin() + index);
        std::copy(oddUVs.begin(), oddUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }

    // -X Face Check
    if (cPos.x < 1 || getBlockInChunk(cPos + vec3(-1, 0, 0)).getBlockType() == Block::BlockType::AIR) {
        std::copy(xNeg.begin(), xNeg.end(), vtx_data.begin() + index);
        std::copy(evenUVs.begin(), evenUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }

    // +Z Face Check
    if (cPos.z >= 15 || getBlockInChunk(cPos + vec3(0, 0, 1)).getBlockType() == Block::BlockType::AIR) {
        std::copy(zPos.begin(), zPos.end(), vtx_data.begin() + index);
        std::copy(evenUVs.begin(), evenUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }

    // -Z Face Check
    if (cPos.z < 1 || getBlockInChunk(cPos + vec3(0, 0, -1)).getBlockType() == Block::BlockType::AIR) {
        std::copy(zNeg.begin(), zNeg.end(), vtx_data.begin() + index);
        std::copy(oddUVs.begin(), oddUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }

    // +Y Face Check
    if (cPos.y >= 15 || getBlockInChunk(cPos + vec3(0, 1, 0)).getBlockType() == Block::BlockType::AIR) {
        std::copy(yPos.begin(), yPos.end(), vtx_data.begin() + index);
        std::copy(evenUVs.begin(), evenUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }

    // -Y Face Check
    if (cPos.y < 1 || getBlockInChunk(cPos + vec3(0, -1, 0)).getBlockType() == Block::BlockType::AIR) {
        std::copy(yNeg.begin(), yNeg.end(), vtx_data.begin() + index);
        std::copy(oddUVs.begin(), oddUVs.end(), uv_data.begin() + index);
        index += faceSideLen;
    }
}

void Chunk::generateVertices(std::vector<Vertex>& vertices, std::vector<UV>& uvs) const {
    // Chunk generation code
    for (int y=0; y<16; y++) {
        for (int z=0; z<16; z++) {
            for (int x=0; x<16; x++) {
                std::array<Vertex, 36> vtx;
                std::array<UV, 36> uvx;

                std::size_t index = 0;

                blockDrawer(vtx, uvx, index, glm::vec3(x, y, z), glm::vec3(x, y, z));

                auto vtx_begin = vtx.begin();
                auto uvx_begin = uvx.begin();

                vertices.insert(vertices.end(), vtx_begin, vtx_begin + index);
                uvs.insert(uvs.end(), uvx_begin, uvx_begin + index);
            }
        }
    }
}