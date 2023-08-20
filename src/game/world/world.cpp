#include "world.hpp"

World::World() {}
World::~World() {}

void World::generateWorld(int xs, int ys, int zs) {
    chunks.clear();

    for (int y=0; y<ys; y++) {
        for (int z=0; z<zs; z++) {
            for (int x=0; x<xs; x++) {
                auto pos = ChunkPos(x, y, z);

                chunks[pos] = Chunk();
            }
        }
    }
}

void World::generateWorldModel(std::vector<Vertex>& vertices, std::vector<UV>& uvs) {
    for (const auto& a : chunks) {
        auto chunkPos = a.first;
        auto& chunk = a.second;

        chunk.generateVertices(vertices, uvs, glm::vec3(std::get<0>(chunkPos), std::get<1>(chunkPos), std::get<2>(chunkPos)));
    }
}