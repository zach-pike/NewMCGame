#pragma once

#include <array>
#include <vector>

#include "block/block.hpp"

#include "../../types/types.hpp"

#include <vector>

class Chunk {
private:
    std::vector<Block> blocks;

    void blockDrawer(std::array<Vertex, 36>& vtx_data, std::array<UV, 36>& uv_data, std::size_t& index, glm::vec3 gPos, glm::vec3 cPos) const;
public:
    Chunk();
    Chunk(Chunk& c);

    ~Chunk();

    void generateVertices(std::vector<Vertex>& vertices, std::vector<UV>& uvs, glm::vec3 chunkPos) const;
};