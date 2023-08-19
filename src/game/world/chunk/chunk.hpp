#pragma once

#include <array>
#include <vector>

#include "block/block.hpp"

#include "../../types/types.hpp"

class Chunk {
private:
    std::array<Block, 16*16*16> blocks;

    void blockDrawer(Vertex* vtx_data, UV* uv_data, std::size_t& index, glm::vec3 gPos, glm::vec3 cPos) const;
public:
    Chunk();
    ~Chunk();

    void generateVertices(std::vector<Vertex>& vertices, std::vector<UV>& uvs) const;
};