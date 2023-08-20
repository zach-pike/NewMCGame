#pragma once

#include <map>
#include <tuple>

#include "chunk/chunk.hpp"

class World {
public:
    using ChunkPos = std::tuple<int, int, int>;
private:
    std::map<ChunkPos, Chunk> chunks;
public:
    World();
    ~World();

    // This could take awhile
    void generateWorld(int xs, int ys, int zs);

    void generateWorldModel(std::vector<Vertex>& vertices, std::vector<UV>& uvs);
};