#pragma once

#include <map>
#include <tuple>

#include "chunk/chunk.hpp"

class World {
public:
    using ChunkPos = std::tuple<int, int, int>;
private:
    std::map<ChunkPos, Chunk> chunks;
    int sizeX;
    int sizeY;
    int sizeZ;

    bool updateFlag = false;
public:
    World();
    ~World();

    Block& getBlock(glm::vec3 pos);

    void setUpdateFlag();
    bool getUpdateFlag();

    // Returns true if the coordinates are in the world
    bool coordinatesInWorld(glm::vec3 pos);

    // This could take awhile
    void generateWorld(int xs, int ys, int zs);

    void generateWorldModel(std::vector<Vertex>& vertices, std::vector<UV>& uvs);
};