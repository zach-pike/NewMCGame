#pragma once

#include <array>
#include <vector>

#include "block/block.hpp"

#include "../../types/types.hpp"

class Chunk {
private:
    std::array<Block, 16*16*16> blocks;
public:
    Chunk();
    ~Chunk();

    void generateVertices(std::vector<Vertex>& vertices, std::vector<UV>& uvs) const;
};