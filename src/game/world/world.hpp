#pragma once

#include <map>
#include <tuple>

#include "chunk/chunk.hpp"

class World {
private:
    std::map<std::tuple<int, int, int>, Chunk> chunks;
public:
    World();
    ~World();

    void generateWorldModel(std::vector<Vertex>& vertices, std::vector<UV>& uvs);
};