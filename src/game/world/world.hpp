#pragma once

#include <map>
#include <tuple>
#include <vector>

#include "chunk/chunk.hpp"

class World {
public:
    using ChunkPos = std::tuple<int, int, int>;
private:
    std::map<ChunkPos, Chunk> chunks;
    int sizeX;
    int sizeY;
    int sizeZ;

    Chunk& getChunkFromWorldCoords(glm::vec3 pos);
public:
    World();
    ~World();

    Block getBlock(glm::vec3 pos);
    void setBlock(glm::vec3 pos, Block block);

    // Returns true if the coordinates are in the world
    bool coordinatesInWorld(glm::vec3 pos);

    // This could take awhile
    void generateWorld(int xs, int ys, int zs);

    std::vector<Chunk*> getChunksReference();

    void update();
};