#include "world.hpp"

#include "../utils/PerlinNoise.hpp"

World::World() {}
World::~World() {}

Chunk& World::getChunkFromWorldCoords(glm::vec3 pos) {
    float chunkX = std::floor(pos.x / 16.f);
    float chunkY = std::floor(pos.y / 16.f);
    float chunkZ = std::floor(pos.z / 16.f);
    return chunks.at(ChunkPos(chunkX, chunkY, chunkZ));
}

Block World::getBlock(glm::vec3 pos) {
    Chunk& chunk = getChunkFromWorldCoords(pos);

    float localX = std::floor(std::fmod(pos.x, 16.f));
    float localY = std::floor(std::fmod(pos.y, 16.f));
    float localZ = std::floor(std::fmod(pos.z, 16.f));

    return chunk.getBlock(glm::vec3(localX, localY, localZ));
}

void World::setBlock(glm::vec3 pos, Block b) {
    Chunk& chunk = getChunkFromWorldCoords(pos);

    float localX = std::floor(std::fmod(pos.x, 16.f));
    float localY = std::floor(std::fmod(pos.y, 16.f));
    float localZ = std::floor(std::fmod(pos.z, 16.f));

    return chunk.setBlock(glm::vec3(localX, localY, localZ), b);
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

                chunks.emplace(pos, std::move(Chunk()));
            }
        }
    }

    const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

    for (int z=0; z<sizeZ*16; z++) {
        for (int x=0; x<sizeX*16; x++) {
            const double yMax = perlin.octave2D_01((x * 0.01), (z * 0.01), 4) * 50 + 20;

            for(int y=0; y < yMax; y++) setBlock(glm::vec3(x, y, z), Block(Block::BlockType::DIRT));
            setBlock(glm::vec3(x, yMax, z), Block(Block::BlockType::GRASS));
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

std::vector<Chunk*> World::getChunksReference() {
    std::vector<Chunk*> chunksL;
    for (auto& kv : chunks) chunksL.push_back(&kv.second);
    return chunksL;
}

void World::update() {
    for (auto& a : chunks) {
        auto& chunk = a.second;
        if (!chunk.pendingMeshUpdate()) continue;

        auto chunkPos = a.first;
        auto chunkCoord = glm::vec3(std::get<0>(chunkPos), std::get<1>(chunkPos), std::get<2>(chunkPos));

        chunk.update(chunkCoord);
    }
}