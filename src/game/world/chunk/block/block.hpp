#pragma once

#include <utility>
#include <glm/glm.hpp>

class Block {
public:
    enum class BlockFace {
        NORTH, // +X
        SOUTH, // -X
        EAST,  // +Z
        WEST,  // -Z
        TOP,   // +Y
        BOTTOM // -Y
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