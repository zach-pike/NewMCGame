#include "world.hpp"

World::World() {}
World::~World() {}

void World::setUpdateFlag() {
    updateFlag = true;
}

bool World::getUpdateFlag() {
    return updateFlag;
}

Block& World::getBlock(glm::vec3 pos) {
    float chunkX = std::floor(pos.x / 16.f);
    float chunkY = std::floor(pos.y / 16.f);
    float chunkZ = std::floor(pos.z / 16.f);

    float localX = std::floor(std::fmod(pos.x, 16.f));
    float localY = std::floor(std::fmod(pos.y, 16.f));
    float localZ = std::floor(std::fmod(pos.z, 16.f));

    auto& chunk = chunks.at(ChunkPos(chunkX, chunkY, chunkZ));

    return chunk.getBlock(glm::vec3(localX, localY, localZ));
}

void World::generateWorld(int xs, int ys, int zs) {
    chunks.clear();

    sizeX = xs;
    sizeY = ys;
    sizeZ = zs;

    for (int y=0; y<ys; y++) {
        for (int z=0; z<zs; z++) {
            for (int x=0; x<xs; x++) {
                auto pos = ChunkPos(x, y, z);

                chunks[pos] = Chunk();
            }
        }
    }
}

bool World::coordinatesInWorld(glm::vec3 pos) {
    float maxX = sizeX * 16;
    float maxY = sizeY * 16;
    float maxZ = sizeZ * 16;

    return pos.x > 0 && pos.x < maxX
        && pos.y > 0 && pos.y < maxY
        && pos.z > 0 && pos.z < maxZ;
}

void World::generateWorldModel(std::vector<Vertex>& vertices, std::vector<UV>& uvs) {
    updateFlag = false;
    for (const auto& a : chunks) {
        auto chunkPos = a.first;
        auto& chunk = a.second;

        chunk.generateVertices(vertices, uvs, glm::vec3(std::get<0>(chunkPos), std::get<1>(chunkPos), std::get<2>(chunkPos)));
    }
}