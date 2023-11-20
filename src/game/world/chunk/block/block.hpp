#pragma once

#include <utility>
#include <glm/glm.hpp>

class Block {
public:
    enum class BlockFace {
        NORTH = 0, // +X
        SOUTH = 1, // -X
        EAST = 2,  // +Z
        WEST = 3,  // -Z
        TOP = 4,   // +Y
        BOTTOM = 5 // -Y
    };
private:
    uint64_t packHash;
    unsigned int blockId;
public:
    Block(unsigned int bt = 0, std::uint64_t packHash = 0);
    Block(std::pair<std::uint64_t, unsigned int> ident);

    ~Block();

    unsigned int getBlockId() const;
    std::pair<std::uint64_t, unsigned int> getIdent() const;
};